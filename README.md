# sm-shell
My own implementation of a basic shell program. A refresher project. 

<smshell> is a simple implementation of a shell in C. It demonstrates the basics of how a shell works. That is: read, parse, fork, exec, and wait. So I think it's pretty cool. Since it's a simple implementation, it has some limitations.

1. Commands must be on a single line.
2. Arguments must be separated by whitespace.
3. No quoting arguments or escaping whitespace.
4. No piping or redirection.
5. Only builtins are: cd, help, exit.

### TO DO
1. Provide custom configuration options
2. Extend built-in shell commands (pipes, find etc)
3. Provide themes
