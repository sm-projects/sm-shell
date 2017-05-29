#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>

#define FFLAG  (O_WRONLY | O_CREAT | O_TRUNC)
#define FMODE  (S_IRUSR | S_IWUSR)


/**
 * Redirects the output of the previous command into  the file opened after the > symbol.
 * Assumes that the command line has been parsed and the outfile name has been extracted.
 */
int  redirectin(char* cmd, char* infile) {

    int infd;
    int err;

    if (infile == NULL) {
        return 0;
    }
    if (infd = open(infile,FFLAG, FMODE) == -1) {
        return -1;
    }
    if (dup2(infd,STDOUT_FILENO) == -1) {
        err =  errno;     //capture the correct error number
        close(infd);
        errno = err;
        return -1;
    }
}
/**
 * Redirects the output of the previous command into  the file opened after the > symbol.
 * Assumes that the command line has been parsed and the outfile name has been extracted.
 */
int  redirectout(char* cmd, char* outfile) {

    int outfd;
    int err;

    if (outfile == NULL) {
        return 0;
    }
    if (outfd = open(outfile,FFLAG, FMODE) == -1) {
        return -1;
    }
    if (dup2(outfd,STDOUT_FILENO) == -1) {
        err =  errno;     //capture the correct error number
        close(outfd);
        errno = err;
        return -1;
    }
}
