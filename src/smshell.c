/**
 *  My own implementation of a shell called smshell.
 *
 * Basic lifecycle of a shell program are the following
 * Initialize the shell
 * Parse and interpret the commands passed to the shell program from stdin
 * Execute the commands and collect the return status
 * cleanup, i.e  free any memory before the shell program terminates
 */
#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<termios.h>
//#include<bits/sigaction.h>
#include<signal.h>
#include "smshell.h"
#include "smutils.h"

static pid_t  smsh_pgid;
static int smsh_pid ;
static int smsh_pgid;
static struct termios shell_tmodes;

/** Implement in-built functions for smshell.  Currently the following functions are
 *  supported
 *            1. change directory
 *            2. help
 *            3. pipe
 */
int smsh_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int smsh_help(char **args) {
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (int i = 0; i < smsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int smsh_exit(char **args) {

    return 0;
}

int smsh_pipe(int write_pid, int read_pid){
    //Pipe takes an array of 2 pids
    int pids[2] = {write_pid, read_pid};

    char buf[30];
    if(pipe(pids) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    return 0;
}

/****************************************** End of shell commands  implementation  ***/


/**
 * Create default shell prompt. Later this can be customized by setting  SMSH_PROMPT variable
 * in the config file  or environment.
 *
 */
void smsh_prompt() {
    char  hostNm[1024] = "";
    gethostname(hostNm, sizeof(hostNm));
    printf("%s@%s %s >",getenv("LOGNAME"), hostNm, "SMSHELL");
}

/**
 * Initialize - check to see the following
 *
 *  1. Check to see if invoked from another shell.
 *  2. If this is a subshell that is running in interactive mode has to check if it has been placed
 *     in the foreground by its parent process- ceck to see the following
 *
 *      1. Check to see if invoked from another shell.
 *      2. If this is a subshell that is running in interactive mode thas check if it has been placed
 *         in the foreground by its parent process
 *
 *   See GNU manual for more details on initializing the shell section
 */
void init_shell() {

    smsh_pid = getpid();
    /** Check for interactive mode. */
    int isInteractive  = isatty(STDIN_FILENO);

    if (isInteractive) {
        //Loop until this new subshell is in the foreground.
        printf("Shell is running in interactive mode. \n");
        while(tcgetpgrp(STDIN_FILENO) != (smsh_pgid = getpgrp())) {
            kill(smsh_pid, SIGTTIN);
        }
        /** Ignore  interactive and other job control signals. */
        act_child.sa_handler = signalHandler_child;
        act_int.sa_handler = signalHandler_int;

        sigaction(SIGCHLD, &act_child, NULL);
        sigaction(SIGINT, &act_int, NULL);

        //Put this process into the same process group
        setpgid(smsh_pid, smsh_pid);
        smsh_pgid = getpgrp();
        if(smsh_pid != smsh_pgid) {
            perror("Could not find the shell in its own process group.");
            exit(1);

        }
        printf("Reached here .....\n");
        //Grab control pf the terminal
        tcsetpgrp(STDIN_FILENO, smsh_pgid);
        // Save the default terminal attributes for shell
        tcgetattr(STDIN_FILENO,&shell_tmodes);
        printf("Reached here .....\n");
    } else {
        printf("Shell is not running in interactive mode.\n");
        exit(1);
    }
}
//////////////////////
// signal handlers
//////////////////////

void signalHandler_child(int p) {
    //Wait for all dead processed. Use a non-blocking call ro make sure that this signal does not block
    //if a child process was cleaned up.
    while(waitpid(-1, NULL,WNOHANG) > 0) {}
    printf("\n");
}

void signalHandler_int(int p) {
    pid = getpid();
    //We send a SIGTERM signal to the child process
    if (kill(smsh_pid,SIGTERM)==0) {
        printf("\nProcess %d received a SIGINT signal\n",pid);
    } else {
        printf("\n");
    }
}


/**
 * Allocates a block (see buffer size) and then reads a line from stdin.
 * If  text entered exceeds the initial block size then it reallocates more space.
 *
 */
char *smshell_read_line(void) {
    int bufSize = SMSHELL_BUFFER_SIZE;
    int pos = 0;
    char *buf = malloc(sizeof(char) * bufSize);
    int  c_in;

    if (!buf) {
        fprintf(stderr, "smshell: failed to allocate enough memory for shell.\n");
        exit(EXIT_FAILURE);
    }
    while(1) {
        //read a char
        c_in = getchar();
        if (c_in == EOF || c_in == '\n') {
            buf[pos] = '\0';
            return buf;
        } else {
            buf[pos] = c_in;
        }
        pos++;

        if (pos >= bufSize) {
            bufSize += SMSHELL_BUFFER_SIZE;
            buf = realloc(buf,bufSize);
            if(!buf) {
               fprintf(stderr, "smshell: failed to allocate enough memory for shell.\n");
               exit(EXIT_FAILURE);
            }
        }
    }

}

/**
 * Does not allow quoting or backslash escaping in command line arguments.
 *
 */
char **smshell_parse_line(char *line) {
   int bufSize = SMSHELL_BUFFER_SIZE;
   int pos = 0;
   char **tokens;
   char *token;

   tokens = malloc(bufSize * sizeof(char*));
   if (!tokens) {
        fprintf(stderr, "smshell: failed to allocate enough memory for shell.\n");
         exit(EXIT_FAILURE);
   }
   token = strtok(line,SMSHELL_TOK_DELIM);
   while(token != NULL) {
       tokens[pos] = token;
       pos++;

       if(pos >= bufSize) {
           bufSize += SMSHELL_BUFFER_SIZE;
           tokens =realloc(tokens, bufSize * sizeof(char*));
           if(!tokens) {
             fprintf(stderr, "smshell: failed to allocate enough memory for shell.\n");
             exit(EXIT_FAILURE);
           }
       }
       token = strtok(NULL,SMSHELL_TOK_DELIM);
   }
   tokens[pos] = NULL;
   return tokens;

}
/**
 *  In order to execute a command, fork a new process and if it succeeds
 *  run execvp  that  creates that executes the command as a child process.
 *
 */
int  smshell_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        //run execvp as a child process.
        if(execvp(args[0],args) == -1) {
            perror("sm-shell, error executing the shell command.");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        //Error forking
        perror("sm-shell- error forking.");
    }else {
        do {
            wpid = waitpid(pid,&status,WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
   return 1;
}

int smshell_execute(char **args){

    //Empty command was entered.
    if(args[0] == NULL)
        return 1;
    //Otherwise check whether build-in shell functions needs to  be called.
    for(int i=0; i < smsh_num_builtins(); i++) {
      if (strcmp(args[0], builtin_str[i]) == 0) {
          return (*builtin_func[i])(args);
      }
    }
    //finally run execute if the command passed is not from built-in funcs
    return smshell_launch(args);

}

void perform_cleanup(){
    //To be implemented
    printf("Performing cleanups!\n");
    printf("exiting smshell - goodbye!\n");
}
/**
 * The basic loop of the shell program will
 *  1. read the command from stdin
 *  2. Parse the command string into a program and args
 *  3. Execute the command and return status.
 */
void smshell_loop() {
    char *line;
    char **args;
    int status;

    do {
        //printf("<smshell> ");
        smsh_prompt();
        line = smshell_read_line();
        args = smshell_parse_line(line);
        status = smshell_execute(args);

        free(line);
        free(args);

    }while (status);
}

void startup_msg() {
    textcolor(BRIGHT,RED,BLACK);
    printf("*********************************************\n");
    printf("*** Welcome to smshell.                   ***\n");
    printf("*********************************************\n");
    printf("*********************************************\n");
    printf("*********************************************\n");
    textcolor(RESET,WHITE,BLACK);
}
int main(int argc, char **argv) {
    //init_shell();
    //Load any configuration file
    //load_config()
    startup_msg();
    //Run the main shell loop
    smshell_loop();
    //Perform any clean ups before exiting the shell.
    perform_cleanup();

    return EXIT_STATUS;

}
