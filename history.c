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
#include <unistd.h>
#include "history.h"

char historyFilePath[1024];

void setHistoryFilePath()
{
	getcwd(historyFilePath, sizeof(historyFilePath));
	strcat(historyFilePath, "/HISTORY");
}

int read_history(char *history[])
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int h_pos = 0;
	fp = fopen(historyFilePath, "r");
	if (fp == NULL)
	{
		perror("No HISTORY file");
		return 0;
	}
	/*
	else
	{
		printf("Reading history from '%s'...\n", historyFilePath);
	}
	*/
	
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
		//printf("H:'%s'\n", line);
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
		if (strncmp(history[i], cmd, strlen(cmd)) == 0) // line already in history
		{
			in_history = 1;
			break;
		}
		/*
		else
		{
			printf("UM:'%s'(%d)|'%s'(%d)\n", history[i], strlen(history[i]), cmd, strlen(cmd));
		}
		*/
	}
	if (in_history == 0)
	{
		FILE * fp;
		
		fp = fopen(historyFilePath, "a");
		if (fp == NULL)
		{
			perror("Unable to write to HISTORY file");
			return 0;
		}
		else
		{
			fprintf(fp, "%s\n", cmd);
			fclose(fp);	
			return 1;
		}
	}
	return 0;
}