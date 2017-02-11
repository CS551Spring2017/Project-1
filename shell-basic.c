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

#define CMD_PART_ARGC 1
#define CMD_PART_ARGV 2

static char *prompt = "shell> ";
static char done = 0;

typedef struct {
	char *cmd;
	char **arg;
	int arg_num;
} shell_cmd;

int parseline();
int shell_process(char *arg);
int verify_parenthesis_count(char *cmd);
shell_cmd parsecmd(char *cmd);
int shell_sequence(char *cmd);
int shell_parallel(char *cmd);

int main()
{
	
	while (!done)
	{
		printf("%s", prompt);
		parseline();
	}
	
	return 0;
}

int parseline()
{
	char *buffer;
	size_t bufsize = 32;
	size_t characters;
	
	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}
	
	int c;
	int i = 0;
	int ready = 0;
	system("/bin/stty raw"); // disable input buffers
	while (!ready)
	{
		c = getchar();
		if (c == '\r' || c == '\n') // 'enter' pressed
		{
			ready = 1;
			buffer[i++] = '\0';
		}
		else if (c == '\t') // 'tab' pressed
		{
			printf("\r\nSuggested: cmd1\r\n           cmd1 -l\r\n           cmd1 -l -h\r\n"); //sample
			printf("%s%s", prompt, buffer);
		}
		else if (c == '.') // emergency exit (temporary)
		{
			system("/bin/stty cooked"); // enable input buffers
			printf("\r\n");
			exit(1);
		}
		else if (c == '\b' || c == 8 || c == 127) // 'backspace' (8) or 'delete' (127)
		{
			if (i > 0)
			{
				printf("\b \b"); // replace char with a space
				i--;
			}
		}
		else if (c == 26 || c == 24 || c == 3 || c == 4) // ctrl + (26=z, 24=x, 3=c, 4=d)
		{
			printf("\r\nkeyboard interrupt!\n");
			
			system("/bin/stty cooked"); // enable input buffers
			printf("\r\n");
			exit(1);
		}
		else {
			printf("%c", c);
			buffer[i++] = c;
		}
	}
	system("/bin/stty cooked"); // enable input buffers
	
	size_t len = strlen(buffer);
	printf(" [I='%s'] [%d chars]\n", buffer, len);
	
	shell_process(buffer);
	
	return 1;
}

int shell_process(char *arg)
{
	// do stuff
	// cmd1
	// cmd1 -v
	// cmd1;cmd2
	// cmd1;cmd2;cmd3
	// cmd1&cmd2
	// cmd1&(cmd2;cmd3)
	// (cmd1;cmd2)&(cmd1;cmd2)
	// cmd1 -f
	// cmd1 -f;cmd -2
	// printf("pointer test: %s\n", arg);
	
	int p_count = verify_parenthesis_count(arg);
	if (!p_count) return 0;
	
	parsecmd(arg);
	
	return 1;
}

int verify_parenthesis_count(char *cmd)
{
	// only allow '(' with matching ')'
	int left = 0;
	int right = 0;
	int i = 0;
	int left_mod = 0;	// ) should always be after a (
	for (i = 0; i <= strlen(cmd); i++)
	{
		if (cmd[i] == '(')
		{
			left++;
			left_mod++;
		}
		else if (cmd[i] == ')')
		{
			if (left_mod <= 0)
			{
				perror("Mismatched parenthesis (invalid close).");
				return 0;
			}
			right++;
			left_mod--;
		}
	}
	if (left == right) return 1;
	
	perror("Mismatched parenthesis (unequal number).");
	return 0;
}

shell_cmd parsecmd(char *cmd)
{
	shell_cmd sc;
	
	int i, j = 0, argv_num = 0;
	int len = strlen(cmd);
	int cmd_part = CMD_PART_ARGC;
	
	for (i = 0; i <= len; i++)
	{
		if (cmd_part == CMD_PART_ARGC)
		{
			if (cmd[i] == ' ')
			{
				// change to argv
				cmd_part = CMD_PART_ARGV;
				j = 0;
			}
			else {
				sc.cmd[j] = cmd[i];
			}
		}
		else if (cmd_part == CMD_PART_ARGV)
		{
			if (cmd[i] == ' ')
			{
				argv_num++;
				j = 0;
			}
			else {
				sc.arg[argv_num][j] = cmd[i];
			}
		}
	}
	sc.arg_num = argv_num;
	
	printf("%s \n", sc.cmd);
	
	return sc;
}

int shell_sequence(char *cmd)
{
	
	return 1;
}

int shell_parallel(char *cmd)
{
	
	return 1;
}