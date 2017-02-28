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
#include <sys/wait.h>
#include "shell.h"
#include "history.h"
#include "parse-tree.h"
#include "profile.h"

static char *prompt = "shell> ";
static char done = 0;

extern pid_t *concurrentpids[100];
extern int pid_index;

int main()
{
	/*
	if (ISMINIX == 1)
	{
		printf("Compiled with ISMINIX = 1.\n");
	}
	else
	{
		printf("Compiled with ISMINIX = 0.\n");
	}
	*/
	
	setHistoryFilePath();
	enableAlarm = loadProfile();
	
	while (!done)
	{
		printf("%s", prompt);
		parseline();
		int i;
		int status;
		for(i = 0; i < pid_index; i++)
		{
			waitpid(concurrentpids[i], &status, 0);
		}
		pid_index = 0;
	}
	
	return 0;
}

int parseline()
{
	char *buffer;
	size_t bufsize = 32;
	
	buffer = (char *)malloc(bufsize * sizeof(char));
	if (buffer == NULL)
	{
		perror("Unable to allocate buffer");
		exit(1);
	}
	
	int c;								// keyboard/tty char
	int i = 0;							// buffer index
	int ready = 0;						// triggered on enter
	char *history[MAXHISTORY];			// create history pointer array
	int history_num_match = read_history(history); // set history
	int history_match_len = 0;			// how long the original buffer was when 'tab' was hit
	int history_last_index = -1;		// last history index
	int k27 = 0;
	//int k91 = 0;
	int last_key_space = 0;				// prevent double spaces
	//int last_key_91 = 0;				// up/down fix
	
	system("/bin/stty raw"); // disable input buffers
	while (!ready)
	{
		c = getchar();
		//printf("[%d]\n", c);
		if (c == 32 || c == ' ')
		{
			// first char cannot be space.
			if (i == 0)
			{
				if (ISMINIX == 1)
				{
					//printf("\b \b"); // replace char with a space (for minix)
				}
				else
				{
					printf("\b"); // for ubuntu, etc.
				}
			}
			else
			{
				if (last_key_space == 1)
				{
					// cannot do multiple spaces
					if (ISMINIX == 1)
					{
						//printf("\b \b"); // replace char with a space (for minix)
					}
					else
					{
						printf("\b"); // for ubuntu, etc.
					}
				}
				else
				{
					if (ISMINIX == 1)
					{
						printf(" ");
					}
					else
					{
						
					}
					last_key_space = 1;
					buffer[i++] = c;
				}
			}
		}
		else
		{
			last_key_space = 0;
			if (c == '\r' || c == '\n') // 'enter' pressed
			{
				ready = 1;
				buffer[i++] = '\0';
				printf("\r\n");
			}
			else if (c == '\t') // 'tab' pressed
			{
				clearline();
				if (history_match_len == 0 && history_last_index == -1) // first time hitting 'tab'
				{
					history_match_len = strlen(buffer);
					
					int j, history_found = 0;
					for (j = 0; j < history_num_match; j++)
					{
						// iterate through history and find next 'match'
						if (strncmp(buffer, history[j], history_match_len) == 0)
						{
							// match
							if (j > history_last_index)
							{
								history_last_index = j;
								strcpy(buffer, history[j]);
								i = strlen(history[j]);
								printf("\r%s%s", prompt, history[j]);
								history_found = 1;
								break;
							}
						}
					}
					if (history_found == 0)
					{
						printf("\r%s%s%s", prompt, buffer, " (no match)");
						//printf("\r%s%s%s", prompt, buffer, " (no match)");
					}
				}
				else
				{
					// rewrite prompt+buffer
					printf("\r%s%s", prompt, buffer);
				}
			}
			else if (c == 65) // up arrow
			{
				if (history_match_len == 0 && history_last_index == -1)
				{
					// tab not yet hit
				}
				else
				{
					clearline();
					int j, history_found = 0;
					for (j = history_last_index - 1; j >= 0; j--)
					{
						// iterate through history and find next 'match'
						if (strncmp(buffer, history[j], history_match_len) == 0)
						{
							// match
							history_last_index = j;
							strcpy(buffer, history[j]);
							i = strlen(history[j]);
							printf("\r%s%s", prompt, history[j]);
							history_found = 1;
							break;
						}
					}
					if (history_found == 0)
					{
						printf("\r%s%s%s", prompt, buffer, " (no match-up)");
						//printf("\r%s%s", prompt, buffer);
					}
				}
			}
			else if (c == 66) // down arrow
			{
				if (history_match_len == 0 && history_last_index == -1)
				{
					// tab not yet hit
				}
				else
				{
					clearline();
					int j, history_found = 0;
					for (j = 0; j < history_num_match; j++)
					{
						// iterate through history and find next 'match'
						if (strncmp(buffer, history[j], history_match_len) == 0)
						{
							// match
							if (j > history_last_index)
							{
								history_last_index = j;
								strcpy(buffer, history[j]);
								i = strlen(history[j]);
								printf("\r%s%s", prompt, history[j]);
								history_found = 1;
								break;
							}
						}
					}
					if (history_found == 0)
					{
						printf("\r%s%s%s", prompt, buffer, " (no match-down)");
						//printf("\r%s%s", prompt, buffer);
					}
				}
			}
			/*
			else if (c == '.') // emergency exit (temporary)
			{
				system("/bin/stty cooked"); // enable input buffers
				printf("\r\n");
				exit(1);
			}
			*/
			else if (c == '\b' || c == 8 || c == 127) // 'backspace' (8) or 'delete' (127)
			{
				if (i >= 0)
				{
					/*
					clearline();
					printf("\r%s%s", prompt, buffer);
					*/
					
					if (ISMINIX == 1)
					{
						printf("\b \b"); // replace char with a space (for minix)
					}
					else
					{
						printf(" \b"); // for ubuntu, etc.
					}
					
					i--;
					/*
					buffer[i--] = '\0';
					clearline(); // hopefully this fixes both
					printf("%s%s", prompt, buffer);
					if (i < 0) i = 0;
					*/
				}
				else
				{
					if (ISMINIX == 1)
					{
						
					}
					else
					{
						printf(" ");
					}
				}
			}
			else if (c == 26 || c == 24 || c == 3 || c == 4) // ctrl + (26=z, 24=x, 3=c, 4=d)
			{
				printf("\r\nkeyboard interrupt!\n");
				
				system("/bin/stty cooked"); // enable input buffers
				printf("\r\n");
				exit(1);
			}
			else if (c == 27)
			{
				k27 = 1;
				//k91 = 0;
				printf("%c", c);
				//printf("%s", "<k27>");
			}
			else if (k27 == 1 && c == 91)
			{
				k27 = 0;
				//k91 = 1;
				printf("\b"); // this is the prelude to up/down arrow
				//printf("%s", "<k91>");
			}
			/*
			else if (k91 == 1 && c == 65)
			{c = getchar();
				// emulate down
				k91 = 0;
				printf("%c%c%c", 27, 91, 66);
				printf("%s", "<down>");
			}
			else if (k91 == 1 && c == 66)
			{c = getchar();
				// emulate up
				k91 = 0;
				printf("%c%c%c", 27, 91, 65);
				printf("%s", "<up>");
			}
			else if (c < 32) // esc=27? (happens on down/up), 26 handled above
			{
				// ignore
			}
			*/
			else
			{
				/*
				if (c == 91 || c == '[')
				{
					// do nothing (up/down)
				}
				*/
				// for windows, don't print %c.
				if (ISMINIX == 1)
				{
					//printf("%c", c); // %c for minix
				}
				else
				{
					// no char output for ubuntu
				}
				//printf("\b[%d]", c); // debugging keycodes
				buffer[i++] = c;
				buffer[i] = '\0'; // weird characters at end...
				clearline(); // hopefully this fixes both
				printf("%s%s", prompt, buffer);
			}	
		}
	}
	system("/bin/stty cooked"); // enable input buffers
	
	//size_t len = strlen(buffer);
	//printf(" [I='%s'] [%d chars]\n", buffer, (int) len);
	
	char buf2[MAXLINE];
	strcpy(buf2, buffer);
	int proc = shell_process(buffer);
	if (proc == 1)
	{
		add_history(history, history_num_match, buf2);
	}
	
	// free history
	for (i = 0; i < history_num_match; i++)
	{
		free(history[i]);
	}
	
	return 1;
}

int shell_process(char *buffer)
{
	int p_count = verify_parenthesis_count(buffer);
	if (!p_count) return 0;
	
	if (!strncmp(buffer, "cd", 2))
	{
		if (strlen(buffer) < 4)
		{
			printf("Unable to change directory, no directory entered.\n");
		}
		else if (isDirectory(buffer + 3))
		{
			printf("Change directory: '%s'.\n", buffer + 3);
			chdir(buffer + 3);
		}
		else
		{
			printf("Unable to change directory, '%s' is not a directory.\n", buffer + 3);
		}
	}
	else
	{
		printf("Input: [%s]\n", buffer);
		char prefix[MAXARGS];
	
		resetGlobal();
		infixToPrefix(buffer,prefix);
		reverse(prefix);
		//puts(prefix);
		
		makeAllCmds(prefix);

		char *s = prefix;
		Tree *tree = makeTree(&s);
		if (tree == NULL)
		{
			perror("Invalid input.");
			return 0;
		}
		traverse(tree);
		release(tree);
	}
	
	return 1;
}

int verify_parenthesis_count(char *buffer)
{
	// only allow '(' with matching ')'
	int left = 0;
	int right = 0;
	int i = 0;
	int left_mod = 0;	// ) should always be after a (
	for (i = 0; i <= strlen(buffer); i++)
	{
		if (buffer[i] == '(')
		{
			left++;
			left_mod++;
		}
		else if (buffer[i] == ')')
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

void clearline()
{
	if (ISMINIX == 1)
	{
		printf("\r                                                                                \r");
		//printf("^[[2K\r");
	}
	else
	{
		printf("\33[2K\r");
	}
}