/**
 *  My own implementation of a shell called smshell.
 *
 */

/**
 * Basic lifecycle of a shell program are the following
 * Initialize the shell
 * Parse and interpret the commands passed to the shell program from stdin
 * Execute the commands and collect the return status
 * cleanup, i.e  free any memory before the shell program terminates
 **/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<termios.h>
#include "smshell.h"




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
//****************************************** End of shell commands  implementation
void init_shell() {
    /** Check for interactive mode. */
    int shell_terminal = STDIN_FILENO;
    int isInteractive  = isatty(shell_terminal);
    if (isInteractive) {
        printf("Shell is running in interactive mode. \n");
    } else {
        printf("Shell is not running in interactive mode.\n");
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
   printf("Current token: %s \n",token);
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
        printf("<smshell> ");
        line = smshell_read_line();
        args = smshell_parse_line(line);
        status = smshell_execute(args);

        free(line);
        free(args);

    }while (status);
}

void startup_msg() {
    printf("*********************************************\n");
    printf("*** Welcome to smshell.                   ***\n");
    printf("*********************************************\n");
    printf("*********************************************\n");
    printf("*********************************************\n");
}
int main(int argc, char **argv) {
    init_shell();
    //Load any configuration file
    //load_config();
    startup_msg();
    //Run the main shell loop
    smshell_loop();
    //Perform any clean ups before exiting the shell.
    perform_cleanup();

    return EXIT_STATUS;

}
