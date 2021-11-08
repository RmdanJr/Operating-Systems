#include <stdio.h>
#include <stdlib.h>

void wunzip(char *filename)
{
 FILE *fp = fopen(filename, "r");
 if (fp == NULL)
 {
  printf("wunzip: cannot open file\n");
  exit(EXIT_FAILURE);
 }
 int count;
 char c;
 while (fread(&count, sizeof(int), 1, fp))
 {
  fread(&c, sizeof(char), 1, fp);
  for (int i = 1; i <= count; i++)
   printf("%c", c);
 }
 fclose(fp);
}

int main(int argc, char *argv[])
{
 if (argc < 2)
 {
  printf("wunzip: file1 [file2 ...]\n");
  exit(EXIT_FAILURE);
 }
 for (int i = 1; i < argc; i++)
  wunzip(argv[i]);
 exit(EXIT_SUCCESS);
}
