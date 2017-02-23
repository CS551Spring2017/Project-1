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

static char *prompt = "shell> ";
static char done = 0;

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
	//int k27 = 0;
	//int k91 = 0;
	
	system("/bin/stty raw"); // disable input buffers
	while (!ready)
	{
		c = getchar();
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
				history_match_len = i;
				
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
					printf("%s", " (no match)");
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
			if (history_match_len == 0)
			{
				// tab not yet hit
			}
			else
			{
				clearline();
				int j, history_found = 0;
				for (j = history_last_index - 1; j > 0; j--)
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
					printf("\r%s%s%s", prompt, buffer, " (no more history - top)");
				}
			}
		}
		else if (c == 66) // down arrow
		{
			if (history_match_len == 0)
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
					printf("\r%s%s%s", prompt, buffer, " (no more history - bottom)");
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
			if (i > 0)
			{
				printf("\b \b"); // replace char with a space (for minix)
				//printf(" \b"); // for ubuntu, etc.
				i--;
			}
			else
			{
				printf(" ");
			}
		}
		else if (c == 26 || c == 24 || c == 3 || c == 4) // ctrl + (26=z, 24=x, 3=c, 4=d)
		{
			printf("\r\nkeyboard interrupt!\n");
			
			system("/bin/stty cooked"); // enable input buffers
			printf("\r\n");
			exit(1);
		}
		/*
		else if (c == 27)
		{
			k27 = 1;
			k91 = 0;
			printf("%s", "<k27>");
		}
		else if (k27 == 1 && c == 91)
		{
			k27 = 0;
			k91 = 1;
			printf("%s", "<k91>");
		}
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
		else {
			// for windows, don't print %c.
			printf("%c", c); // %c for minix
			// no char output for ubuntu
			//printf("\b[%d]", c); // debugging keycodes
			
			buffer[i++] = c;
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
	
	return 1;
}

int shell_process(char *buffer)
{
	int p_count = verify_parenthesis_count(buffer);
	if (!p_count) return 0;
	
	char prefix[MAXARGS];
	
	resetGlobal();
	infixtoprefix(buffer,prefix);
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
	printf("\33[2K\r");
}