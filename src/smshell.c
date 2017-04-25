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

//Buffer size for read line
#define SMSHELL_BUFFER_SIZE 1024;

#define EXIT_STATUS  0;

//Not implemented for now
void  load_config() {}

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
        args = smshell_parse_line();
        status = smshell_execute(args);

        free(line);
        free(args);

    }while (status);
}

int main(int argc, char **argv) {
    //Load any configuration file
    load_config();
    //Run the main shell loop
    smshell_loop();
    //Perform any clean ups before exiting the shell.
    perform_cleanup();

    return EXIT_STATUS;

}
