# Unix Utilities

In this project, I have built a few different UNIX utilities, simple versions of commonly used commands (e.g. cat, grep, zip, unzip). I call each of them a slightly different name to avoid confusion; for example, instead of **cat**, I've implemented **wcat** (i.e. "wisconsin" cat).


Summary of what I have done:

- A bunch of single .c files for each of the utilities below: **wcat.c**, **wgrep.c**, **wzip.c**, and **wunzip.c**.
- Each have compiled successfully with the **-Wall** and **-Werror** flags.
- Each have passed all the tests.

## wcat

The program **wcat** is a simple program. Generally, it reads a file or multiple files as specified by the user and prints its contents. A typical usage is as follows, in which the user wants to see the contents of main.c, and thus types:

```
~$ ./wcat main.c
#include <stdio.h>
...
```

To create the **wcat** binary, compile **wcat.c** program using the following command:

```
~$ gcc -o wcat wcat.c -Wall -Werror
```

This will make a single _executable binary_ called **wcat** which you can then run as above.

## wgrep

The second utility you will build is called **wgrep**, a variant of the UNIX tool **grep**. This tool looks through a file, line by line, trying to find a user-specified search term in the line. If a line has the word within it, the line is printed out, otherwise it is not.

Here is how a user would look for the term **foo** in the file **bar.txt**:

```
~$ ./wgrep foo bar.txt
this line has foo in it
so does this foolish line; do you see where?
even this line, which has barfood in it, will be printed.
```

## wzip and wunzip

The third and forth UNIX utilities, is (**wzip**) which is a file compression tool, and the other (**wunzip**) is a file decompression tool.

The type of compression used here is a simple form of compression called _run-length encoding_ (_RLE_). RLE is quite simple: when you encounter **n** characters of the same type in a row, the compression tool (**wzip**) will turn that into the number **n** and a single instance of the character.


Note that typical usage of the **wzip** tool would thus use shell redirection in order to write the compressed output to a file. For example, to compress the file **file.txt** into a smaller **file.z**,
you would type:

```
~$ ./wzip file.txt > file.z
```

The **wunzip** tool simply does the reverse of the **wzip** tool, taking in a compressed file and writing (to standard output again) the uncompressed results. For example, to see the contents of **file.txt**, you would type:

```
~$ ./wunzip file.z
```