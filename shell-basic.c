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

#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 100
#define MAX_COMMAND_LENGTH 100

static char *prompt = "shell> ";
static char done = 0;

int parseline();
int shell_process(char *arg);
int verify_parenthesis_count(char *cmd);
int shell_sequence(char *cmd);
int shell_parallel(char *cmd);
int parseCmd(char *cmdline, char **argv);
void loadProfile();

char path[100];

int main()
{
	loadProfile();
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
			buffer[i++] = ' ';
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
		else {
			printf("\b%c", c);
			buffer[i++] = c;
		}
	}
	system("/bin/stty cooked"); // enable input buffers
	
	size_t len = strlen(buffer);
	printf(" [I='%s'] [%d chars]\n", buffer, len);
	shell_process(buffer);
	//buffer[i] = ' ';buffer[i+1] = '\0';
	shell_sequence(buffer);
	
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

int parseCmd(char *command, char **argv){
	char *p2 = strtok(command, " ");
	int i = 0;

	argv[i++] = p2;
	//printf("Cmd: %s\n", p2);

	while ((p2 = strtok(NULL, " ")) != NULL)
	{
		if (i < MAX_ARGS)
		{
			argv[i++] = p2;
			//printf("Arg(s): %s\n", p2);
		}
		else
		{
			perror("Too many arguments.\n");
		}
	}
	argv[i] = NULL;
}
int shell_sequence(char *cmd)
{
	/**
	printf("You entered the sequence function\n");
	char *argv[MAX_ARGS];
	printf("cmd: %s\n",cmd);
	int argc = parseCmd(cmd, argv);
	printf("argv[0]: %s\n", argv[0]);
	
	int status;
	pid_t pid1 = fork();
	if(pid1 == 0)
	{
		execv(argv[0],argv);
	}
	else{
		waitpid(pid1, &status,0);
	}
	**/

	char *cmd1, *cmd2;
	int i1 = 0;
	int i2 = 0;
	int foundSemi = 0;
	
	cmd1 = (char *)malloc(32 * sizeof(char));
	cmd2 = (char *)malloc(32 * sizeof(char));
	int i;
	for(i = 0; i <= strlen(cmd); i++){
		if(cmd[i] == ';')
		{
			foundSemi = 1;
			continue;
		}
		if(!foundSemi)
		{
			cmd1[i1] = cmd[i];
			i1++;
		}
		else
		{
			cmd2[i2] = cmd[i];
			i2++;
		}
	}
	if(!foundSemi)
	{
		return 0;
	}
	cmd1[i1] = ' ';
	cmd2[i2] = ' ';

	char *argv1[MAX_ARGS];
	parseCmd(cmd1, argv1);
	char *argv2[MAX_ARGS];
	parseCmd(cmd2, argv2);	
	printf("cmd1: %s\n", cmd1);
	printf("cmd2: %s\n", cmd2);
	printf("argv1[0]: %s\n", argv1[0]);
	printf("argv2[0]: %s\n", argv2[0]);
	
	int status;
	pid_t pid1 = fork();
	if(pid1 == 0)
	{
		char *testarg[] = {"/bin/ls",NULL};
		execvp(argv1[0],argv1);
		printf("%s: Command not found\n", argv1[0]);
		exit(0);
	}
	else{
		waitpid(pid1, &status,0);
	}

	pid_t pid2 = fork();
	if(pid2 == 0)
	{
		execvp(argv2[0],argv2);
		printf("%s: Command not found\n", argv2[0]);
		exit(0);
	}
	else{
		waitpid(pid2, &status,0);
	}
	return 1;
}

int shell_parallel(char *cmd)
{
	
	return 1;
}

void loadProfile()
{
	FILE *profile;
	char *buffer;
	size_t bufsize = 100;
	size_t characters;
	profile = fopen("PROFILE", "r");
	if (profile == NULL)
	{
        perror("PROFILE not found");
		return;
	}
	characters = getline(&buffer, &bufsize, profile);
	printf("PATH set to: %s", buffer);
	putenv(buffer);
	characters = getline(&buffer, &bufsize, profile);
	printf("HOME set to: %s\n",buffer+5);
	chdir(buffer+5);
	fclose(profile);
}