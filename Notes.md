# Notes

### Forking a process
When creating a new child process using the fork command, the child process  is an exact duplicate of the
parent process. However some the following subtle differences remain

1. When the fork action is performed the child receives duplicates of all of the parent’s file descriptors. These duplicates are made such that the childi’s  duplicate file descriptors also points to all the files of the opened by the parent. Hence if the child process updates the fide descriptors it is visible to the parent.

### Resources
1. [Linux Man Pages](https://linux.die.net/man/)
2. [LXR](http://lxr.linux.no/#linux+v4.10.1/)
3. [Linux Programming Interface]()
4. [Modular programming in C](http://www.icosaedro.it/c-modules.html)
5. [Using the dup(i) system call](http://edusagar.com/articles/view/25/IO-redirection-using-dup-system-call)
