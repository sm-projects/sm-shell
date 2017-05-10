# sm-shell
My own implementation of a basic shell program. A refresher project. 

<b>smshell</b> is a simple implementation of a shell in C. It demonstrates the basics of how a shell works. That is: read, parse, fork, exec, and wait. So I think it's pretty cool. Since it's a simple implementation, it has some limitations.

1. Commands must be on a single line.
2. Arguments must be separated by whitespace.
3. No quoting arguments or escaping whitespace.
4. No piping or redirection.
5. Only builtins are: cd, help, exit.

### TO DO
1. Provide custom configuration options for prompts, set environment variables
2. Extend built-in shell commands (pipes, history, re-direct, find etc)
3. Redirect STDIN and STDOUT for the new processes by using < and >
4. Place commands in the background with an & at the end of the command line
5. Repeat a previously issued command by typing !number 
6. Provide output in colors

### Notes
1. Read [this article](http://www.linuxjournal.com/article/8603) to print in color.

### Resources
1. [Kernel Development](https://brennan.io/2016/11/14/kernel-dev-ep3/)
2. [Implementing a Shell](https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell)
3. [C-throw the switch](http://www.throwtheswitch.org/)
4. [Simple C Shell](https://github.com/jmreyes/simple-c-shell)
