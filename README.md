# sm-shell
My own implementation of a basic shell program. A refresher project. 

<smshell> is a simple implementation of a shell in C. It demonstrates the basics of how a shell works. That is: read, parse, fork, exec, and wait. So I think it's pretty cool. Since it's a simple implementation, it has some limitations.

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

[](https://brennan.io/2016/11/14/kernel-dev-ep3/)
