#include "libs/cvector.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief prints error message to stderr
 */
void print_error()
{
 char error_message[] = "An error has occurred\n";
 write(STDERR_FILENO, error_message, strlen(error_message));
}

/**
 * @brief checks if the given line is a valid command in terms of directing output to a file.
 * @param line The line of input from the user.
 * @param filename The name of the file to direct the output to.
 * If NULL, line contains multiple filenames, mutiple '>', or no filename.
 * @return True if there is a '>' in the line, false otherwise.
 */
bool redirect_output(char *line, char **filename)
{
 // Check if the line contains a '>'
 if (strchr(line, '>') == NULL)
  return false;
 // from: string before the '>', to: string after the '>'
 char *from = strtok(line, ">");
 char *to = strtok(NULL, "\n");
 // handle the case where there's nothing after '>'
 if (to == NULL)
  return true;
 // handle the case where there's multiple '>'s
 if (strchr(to, '>') != NULL)
  return true;
 int filenum = 0;
 // remove trailing spaces
 char *token = strtok(to, " \0");
 // if the token is not blank, then it's a filename
 if (token != NULL)
 {
  (*filename) = malloc(sizeof(char) * strlen(token));
  strcpy(*filename, token);
 }
 // handle the case where there's multiple files
 while (token != NULL)
 {
  filenum++;
  token = strtok(NULL, " \0");
 }
 // if there's more than one file, then it's an error
 if (filenum != 1)
 {
  free(*filename);
  (*filename) = NULL;
  return true;
 }
 return true;
}

/**
 * @brief parse command to generate array of command arguments & handle command program path & command name.
 * @param cmd command to parse
 * @param path inital array of paths to search for command
 * @param cmdName command name
 * @return array of command arguments
 */
char **parse_cmd(char *cmd, char **path, char *cmdName)
{
 char **cmdArgs = NULL;
 // replace newline char with null char
 if (cmd[strlen(cmd) - 1] == '\n')
  cmd[strlen(cmd) - 1] = 0;
 // split cmd into tokens
 char *tmp = strtok(cmd, " \0");
 if (tmp == NULL)
  return cmdArgs;
 // first token is the command name
 strcpy(cmdName, tmp);
 // create path to command program
 char *token = malloc(sizeof(char) * 1024);
 size_t size = cvector_size(path);
 for (int i = 0; i < size; i++)
 {
  strcpy(token, path[i]);
  strcat(token, "/");
  strcat(token, tmp);
  // if command program exists in path -> break
  if (access(token, X_OK) != -1)
   break;
 }
 // create array of command arguments
 while (token != NULL)
 {
  cvector_push_back(cmdArgs, token);
  token = strtok(NULL, " \0");
 }
 free(token);
 return cmdArgs;
}

/**
 * @brief splits parallel command into individual commands
 * @param cmd the command to split
 * @param parallel true if command is parallel, false otherwise
 * @return array of commands
 */
char **flatten_parallel_commands(char *cmd, bool *parallel)
{
 char **cmdLines = NULL;
 // if && found in cmd, split it into separate commands
 if (strchr(cmd, '&') != NULL)
  (*parallel) = true;
 // relplace newline with null terminator
 if (cmd[strlen(cmd) - 1] == '\n')
  cmd[strlen(cmd) - 1] = 0;
 // split cmd into separate commands & store in cmdLines
 char *token = strtok(cmd, "&");
 while (token != NULL)
 {
  cvector_push_back(cmdLines, token);
  token = strtok(NULL, "&");
 }
 free(token);
 return cmdLines;
}

/**
 * @brief execute a command
 * @param cmdArgs the command and its arguments
 * @param redirect true: redirect output to file, false: redirect output to stdout
 * @param fileName the file name to redirect output to
 */
void execute_cmd(char **cmdArgs, bool is_redirect, char *filename, bool parallel)
{
 // fork a child process for new command
 int rc = fork();
 if (rc < 0) // fork faild
 {
  fprintf(stderr, "fork failed\n");
  exit(EXIT_FAILURE);
 }
 else if (rc == 0) // child process
 {
  // command arguments array should be null terminated
  cvector_push_back(cmdArgs, NULL);
  int fd = 0;
  // handle output direction
  if (is_redirect)
  {
   int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
   dup2(fd, STDOUT_FILENO);
  }
  // execute command
  int status_code = execv(cmdArgs[0], cmdArgs);
  // if execv failed
  if (status_code == -1)
   print_error();
  if (is_redirect)
   close(fd);
  exit(EXIT_SUCCESS);
 }
 else // parent process
 {
  if (parallel)
  {
   int status_code = 0;
   wait(&status_code);
  }
 }
}

/**
 * @brief Checks if the command is valid and executes it
 * @param cmdArgs The command to execute
 */
void changeDirectory(char **cmdArgs)
{
 size_t size = cvector_size(cmdArgs);
 if (size == 2 && chdir(cmdArgs[1]) == 0)
  return;
 print_error();
}

/**
 * @brief replace old paths with new paths
 * @param cmdArgs the command line arguments(new paths)
 * @param path array of old paths
 */
void addPath(char **cmdArgs, char **path)
{
 size_t new_path_num = cvector_size(cmdArgs) - 1;
 size_t cur_path_num = cvector_size(path);
 // remove all paths
 while (cur_path_num != 0)
 {
  cvector_pop_back(path);
  cur_path_num--;
 }
 // add new paths
 for (size_t i = 0; i < new_path_num; i++)
 {
  char *tmp = malloc(sizeof(char) * 512);
  strcpy(tmp, cmdArgs[i + 1]);
  cvector_push_back(path, tmp);
 }
 // assert that number of paths is valid
 assert(cvector_size(path) == new_path_num);
}

int main(int argc, char *argv[])
{
 char **path = NULL;
 bool interactive = false;
 FILE *input = NULL;
 char buffer[1024];
 char cmdName[1024];
 // initialize path
 cvector_push_back(path, "/bin");
 cvector_push_back(path, "/usr/bin");
 // handle different modes
 if (argc == 1) // interactive mode
 {
  interactive = true;
  input = stdin;
 }
 else if (argc == 2) // batch mode
 {
  input = fopen(argv[1], "r");
  if (input == NULL)
  {
   print_error();
   exit(EXIT_FAILURE);
  }
 }
 else
 {
  print_error();
  exit(EXIT_FAILURE);
 }
 // print prompt
 if (interactive)
  printf("wish> ");
 while (fgets(buffer, sizeof(buffer), input) != NULL)
 {
  // print prompt
  if (interactive)
   printf("wish> ");
  // split buffer into multiple commands
  bool parallel = false;
  char **cmdlines = flatten_parallel_commands(buffer, &parallel);
  size_t ncmd = cvector_size(cmdlines);
  // for every command in buffer
  for (size_t i = 0; i < ncmd; i++)
  {
   char *filename = NULL;
   // get output stream (file or stdout)
   bool redirect = redirect_output(cmdlines[i], &filename);
   // command is not valid
   if ((redirect == true) && (filename == NULL))
   {
    print_error();
    continue;
   }
   // parse command
   char **cmdArgs = parse_cmd(cmdlines[i], path, cmdName);
   if (cmdArgs == NULL)
    continue;
   // execute command
   if (strcmp(cmdName, "exit") == 0)
    exit(EXIT_SUCCESS);
   else if (strcmp(cmdName, "cd") == 0)
    changeDirectory(cmdArgs);
   else if (strcmp(cmdName, "path") == 0)
    addPath(cmdArgs, path);
   else
    execute_cmd(cmdArgs, redirect, filename, parallel);
   cvector_free(cmdArgs);
  }
  for (size_t i = 0; i < ncmd; i++)
   wait(NULL);
  cvector_free(cmdlines);
 }
 exit(EXIT_SUCCESS);
}