
# Unix Shell - Wish
This is a basic Unix shell *(CLI)*.

### Basic Shell
The shell can be invoked with either no arguments or a single argument; anything else is an error. 

The no-argument-way is called *interactive mode*. It allows the user to type commands directly.

Here is the no-argument way:

```
prompt> ./wish
wish> 
```
The shell also supports a *batch mode*, which instead reads input from a batch file and executes commands from therein.
Here is how you run the shell with a batch file `batch.txt`:

```
prompt> ./wish batch.txt
```

One difference between batch and interactive modes: in interactive mode, a prompt is printed (`wish> `). In batch mode, no prompt printed.


### Execute Commands

Whenever shell accepts a command, it checks whether the command exists in *search paths* or not. If it does, shell creates a new process to run it. If it does not, shell prints an error message.

### Paths
Search paths are the directories where shell searches for commands. The default search path is `/bin` and `/usr/bin`. User can update paths by using the `path` command.

### Redirection

Shell allows user to redirect the output of a program to a file rather than to the screen with the `>` character. The user can specify the file name to redirect to.

For example, if a user types `ls -la /tmp > output`, nothing printed on the screen. Instead, the standard output of the `ls` program redirected to the file `output`.


### Parallel Commands

Shell allows the user to launch parallel commands. This is accomplished with the ampersand operator as follows:

```
wish> cmd1 & cmd2 args1 args2 & cmd3 args1
```