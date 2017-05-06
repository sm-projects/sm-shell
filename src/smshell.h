#define  TRUE 1
#define  FALSE !TRUE

static  int SMSH_IS_INTERACTIVE;
//Buffer size for read line
#define SMSHELL_BUFFER_SIZE 1024;
#define EXIT_STATUS 0;
#define SMSHELL_TOK_DELIM " \t\r\n\a"


//*********************** Inbuilt shell commands *****************************
static int smsh_cd(char **args);
static int smsh_help(char **args);
static int smsh_exit(char **args);


/*
  List of builtin commands, followed by their corresponding functions.
 */
static char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};


static int (*builtin_func[]) (char **) = {
  &smsh_cd,
  &smsh_help,
  &smsh_exit
};

int smsh_num_builtins() {
    return sizeof(builtin_str)/sizeof(char *);
}
