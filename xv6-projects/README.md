
# Xv6 Projects

These projects were implemented *inside* Xv6 kernel. Xv6 kernel is a port of the original Unix (version 6), and is runnable on modern x86 processors. It was developed at [MIT](https://pdos.csail.mit.edu/6.828/2017/xv6.html).


## 1. ```getreadcount()``` system call
The goal of the project is to add a system call to xv6. **getreadcount()** system call, simply returns how many times that the **read()** system call has been called by user processes since the time that the kernel was booted.