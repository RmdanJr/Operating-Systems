#include <stdio.h>
#include <stdlib.h>

void wcat(char *fname)
{
 FILE *fp;
 char ch;
 fp = fopen(fname, "r");
 if (fp == NULL)
 {
  printf("wcat: cannot open file\n");
  exit(EXIT_FAILURE);
 }
 while ((ch = getc(fp)) != EOF)
  putchar(ch);
 fclose(fp);
}

int main(int argc, char *argv[])
{
 for(int i = 1;i < argc;i++)
  wcat(argv[i]);
 exit(EXIT_SUCCESS);
}
