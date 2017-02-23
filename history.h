/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#define MAXHISTORY 64
#define MAXHISTORYLEN 512

int read_history(char *history[]);
int add_history(char *history[], int history_num, char *cmd);