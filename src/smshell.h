#define  TRUE 1
#define  FALSE !TRUE

//Buffer size for read line
#define SMSHELL_BUFFER_SIZE 1024;  //Maximum number of chars in the command line.
#define EXIT_STATUS 0;
#define SMSHELL_TOK_DELIM " \t\r\n\a"



//Buffer size for read line
//*********************** Inbuilt shell commands *****************************
int smsh_cd(char **args);
int smsh_help(char **args);
int smsh_exit(char **args);


/*
 *  List of builtin commands, followed by their corresponding functions.
*/
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};


int (*builtin_func[]) (char **) = {
  &smsh_cd,
  &smsh_help,
  &smsh_exit
};

int smsh_num_builtins() {
    return sizeof(builtin_str)/sizeof(char *);
}

struct sigaction act_child;
struct sigaction act_int;


void signalHandler_child(int p);
void signalHandler_int(int p);

pid_t  pid; //current process id
