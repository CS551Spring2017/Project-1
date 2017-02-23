/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

int read_history(char *history[])
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int h_pos = 0;
	fp = fopen("HISTORY", "r");
	if (fp == NULL)
	{
		perror("No HISTORY file");
		return 0;
	}
	
	
	while ((read = getline(&line, &len, fp)) != -1)
	{
		if (h_pos == MAXHISTORY)
		{
			perror("History too long, ignoring rest.");
			history[h_pos++] = NULL;
		}
		if (h_pos >= MAXHISTORY)
		{
			continue;
		}
		line[strlen(line) - 1] = '\0'; // otherwise line = 'cmd1                  ' or similar
		//printf("%d%s\n", h_pos++, line);
		history[h_pos] = (char *)malloc(MAXHISTORYLEN * sizeof(char));
		strcpy(history[h_pos++], line);
	}
	free(line);
	fclose(fp);	
	
	return h_pos;
}
int add_history(char *history[], int history_num, char *cmd)
{
	int in_history = 0, i;
	for (i = 0; i < history_num; i++)
	{
		if (!strcmp(history[i], cmd)) // line already in history
		{
			in_history = 1;
			break;
		}
	}
	if (!in_history)
	{
		FILE * fp;
		
		fp = fopen("HISTORY", "a");
		if (fp == NULL)
		{
			perror("Unable to write to HISTORY file");
			return 0;
		}
		else
		{
			fprintf(fp, "%s\n", cmd);
			return 1;
		}
	}
	return 0;
}