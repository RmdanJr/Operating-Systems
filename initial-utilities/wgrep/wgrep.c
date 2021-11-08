#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grep_line(char *line, char *pattern)
{
 if (strstr(line, pattern) != NULL)
  printf("%s", line);
}

void grep_file(char *filename, char *pattern)
{
 char *line = NULL;
 size_t len = 0;
 FILE *fp = fopen(filename, "r");
 if (fp == NULL)
 {
  printf("wgrep: cannot open file\n");
  exit(EXIT_FAILURE);
 }
 while (getline(&line, &len, fp) != EOF)
 {
  grep_line(line, pattern);
 }
 fclose(fp);
}

int main(int argc, char *argv[])
{
 if (argc < 2)
 {
  printf("wgrep: searchterm [file ...]\n");
  exit(EXIT_FAILURE);
 }
 else if(argc == 2)
 {
  grep_file("/dev/stdin", argv[1]);
 }
 else {
  for (int i = 2; i < argc; i++)
  {
   grep_file(argv[i], argv[1]);
  }
 }
 exit(EXIT_SUCCESS);
}
