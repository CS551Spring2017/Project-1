/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#define CMD_PART_ARGC 1
#define CMD_PART_ARGV 2
#define MAXLINE       1024
#define MAXHISTORY    64
#define ISMINIX       1

int parseline();
int shell_process(char *arg);
int verify_parenthesis_count(char *cmd);
void clearline();