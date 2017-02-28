/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "parse-tree.h"
#include "profile.h"

//Global Vars

int top = -1;
int globalCounter = 0;
char stack[MAX];
int pointerCheck = 0;
pid_t *concurrentpids[100];
int pid_index = 0;

typedef struct {
	char *argv[MAXARGS];
} cmd_struct;

cmd_struct all_cmds[MAX];

struct node {
	char * op;
	cmd_struct cmd;
	struct node *left;
	struct node *right;
};

typedef struct node Tree;

//main Asks for infix expression -> builds prefix array -> builds prefix parse tree
/*
(ls -l;ps -l)&(man&htop)
& ; ls -l ps -l & man top 

*/
/*

int main() {
	char infix[MAXARGS],prefix[MAXARGS],temp;
	
	printf("Enter infix operation: ");
	gets(infix);
	
	infixToPrefix(infix,prefix);
	reverse(prefix);
	puts(prefix);
	
	makeAllCmds(prefix);

	char *s = prefix;
	Tree *tree = makeTree(&s);
	//printInfix(tree);
	traverse(tree);
	release(tree);
	
	return(1);
}
*/

void resetGlobal()
{
	top = -1;
	globalCounter = 0;
	pointerCheck = 0;
}

char *getToken(char **p)
{
	
	while (**p && isspace(**p))
	{
		++*p;
	}
	
	char *ret = *p;
	
	if (!*ret)
	{
		return NULL;
	}
	
	while (*ret == '*')
	{
		//printf("found arg\n");
		//printf("%s\n", ret);
		while (**p && !isspace(**p))
		{
			++*p;
		}
		++*p;
		ret = *p;
	}
	
	if (*ret == '*')
	{
		
	}
	else
	{
		while (**p && !isspace(**p))
		{
			++*p;
		}
	}

	if (!**p)
	{
		**p = 0;
		++*p;	
	}
	
	return ret;
}

Tree *makeTree(char **p)
{
	char *token = getToken(p);
	//printf("%s\n", token);
	if (token)
	{
		Tree *tree = malloc(sizeof(*tree));
		cmd_struct temp;
		switch (*token)
		{
			case ';':
			case '&':
				tree->op = token;
				tree->cmd = temp;
				tree->left = makeTree(p);
				tree->right = makeTree(p);
				break;
			default:
				tree->op = "NULL";
				tree->cmd = all_cmds[globalCounter];
				//printf("%s\n", all_cmds[globalCounter].argv[0]);
				tree->left = NULL;
				tree->right = NULL;
				++globalCounter;
		}
		return tree;
	}
	return NULL;
}

void release(Tree *tree)
{
	if (tree)
	{
		release(tree->left);
		release(tree->right);
		free(tree);
	}
}

void printInfix(Tree *tree)
{
	if (tree)
	{
		if (!(strcmp(tree->op,";")) || !(strcmp(tree->op,"&")))
		{
			putchar('(');
			printInfix(tree->left);
			printf(" %s ", tree->op);
			printInfix(tree->right);
			putchar(')');
		}
		else
		{
			if (tree->cmd.argv[1] == NULL)
			{
				printf("%s ", tree->cmd.argv[0]);
			}
			else
			{
				int k = 1;
				printf("%s ", tree->cmd.argv[0]);
				//printf("Args:");
				while (tree->cmd.argv[k])
				{
					printf("%s ",tree->cmd.argv[k]);
					++k;
				}
			}
		}
	}
}

void makeAllCmds(char *prefix)
{
	int i = 0;
	int prevCmd = 0;
	int arg = 0;
	char * token = strtok(prefix," ");
	while (token)
	{
		if (isOperatorString(token) == 0)
		{
			if (token[0] == '*')
			{
				++arg;
				token++;
				all_cmds[prevCmd].argv[arg] = token;
			}
			else
			{
				all_cmds[prevCmd].argv[++arg] = NULL;
				arg = 0;
				all_cmds[i].argv[0] = token;
				prevCmd = i;
				++i;
			}
		}
		token = strtok(NULL," ");
	}
}

void infixToPrefix(char infix[MAXARGS], char prefix[MAXARGS])
{
	int i,j=0;
	char symbol;
	stack[++top]='#';
	reverse(infix);
	for (i = 0; i < strlen(infix); i++)
	{
		symbol = infix[i];
		if (isOperator(symbol) == 0)
		{
			prefix[j] = symbol;
			j++;
			if (i + 1 != strlen(infix))
			{
				char temp = infix[i + 1];
				if (isNonParenOperator(temp) == 1)
				{
					prefix[j] = ' ';
					j++;
				}
				if (i+2 != strlen(infix))
				{
					char temp2 = infix[i+2];
					if (!isOperator(temp2))
					{
						if (temp == ' ')
						{
							prefix[j] = '*';
							j++;
						}
					}
				}
			}
		}
		else
		{
			if (symbol == ')')
			{
				push(symbol);
			}
			else if (symbol == '(')
			{
				while (stack[top] != ')')
				{
					prefix[j] = ' ';
					j++;
					prefix[j] = pop();
					j++;
				}
				prefix[j] = ' ';
				j++;
				pop();
			}
			else
			{
				if (prcd(stack[top]) <= prcd(symbol))
				{
					push(symbol);
				}
				else
				{
					while (prcd(stack[top]) >= prcd(symbol))
					{
						prefix[j] = ' ';
						j++;
						prefix[j] = pop();
						j++;
					}
					push(symbol);
				}
				//end for else
			}
		}
		//end for else		
	}
	//end for for
	while (stack[top] != '#')
	{
		prefix[j] = ' ';
		j++;
		prefix[j] = pop();
		j++;
	}
	prefix[j] = '\0';
}


void reverse(char array[MAXARGS]) // for reverse of the given expression 
{
	int i,j;
	char temp[MAXARGS];
	for (i = strlen(array)-1, j=0; i + 1 != 0; --i, ++j)
	{
		temp[j] = array[i];
	}
	temp[j] = '\0';
	strcpy(array, temp);
}


char pop()
{
	char a;
	a = stack[top];
	top--;
	return a;
}


void push(char symbol)
{
	top++;
	stack[top] = symbol;
}


int prcd(char symbol)
{
	switch (symbol)
	{
		case '&':
		case ';':
			return 2;
			break;
		case '#':
		case '(':
		case ')':
			return 1;
			break;
		default:
			return 0;
	}
}


int isOperator(char symbol)
{
	switch (symbol)
	{
		case '&':
		case ';':
		case '(':
		case ')':
			return 1;
			break;
		default:
			return 0;
	}
}

int isNonParenOperator(char symbol)
{
	switch (symbol)
	{
		case '&':
		case ';':
			return 1;
			break;
		default:
			return 0;
	}
}

int isOperatorString(char * symbol)
{
	if (!strcmp(symbol, ";"))
	{
		return 1;
	}
	else if (!strcmp(symbol, "&"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void traverse(Tree *tree)
{
	if (tree == NULL)
	{
		printf("Invalid input\n");
		exit(1);
	}
	//CODE FOR PARALLEL
	if (!strcmp(tree->op, "&")) //Is the root operator &?
	{
		if (isOperatorString(tree->right->op)) //Is the RIGHTside a command?
		{
			pid_t childshell = fork(); //Make a new shell
			concurrentpids[pid_index] = childshell;
			pid_index++;
			if (childshell == 0)
			{
				traverse(tree->right);
				exit(0);
			}
		}
		else //Rightside IS a command
		{
			pid_t cmd = fork();
			concurrentpids[pid_index] = cmd;
			pid_index++;
			if (cmd == 0)
			{
				execvp(tree->right->cmd.argv[0],tree->right->cmd.argv);
				printf("%s: command not found\n", tree->right->cmd.argv[0]);
				exit(1);
			}
			else
			{
				spawnMonitor(cmd);
			}
		}
		if (isOperatorString(tree->left->op)) //Is the LEFTside a command?
		{
			pid_t childshell = fork(); //Make a new shell
			concurrentpids[pid_index] = childshell;
			pid_index++;
			if (childshell == 0)
			{
				traverse(tree->left);
				exit(0);
			}
		}
		else //Leftside IS a command
		{
			pid_t cmd = fork();
			concurrentpids[pid_index] = cmd;
			pid_index++;
			if (cmd == 0)
			{
				execvp(tree->left->cmd.argv[0],tree->left->cmd.argv);
				printf("%s: command not found\n", tree->left->cmd.argv[0]);
				exit(1);
			}
			else
			{
				spawnMonitor(cmd);
			}
		}
	}
	//CODE FOR SEQUENCE
	else if (!strcmp(tree->op, ";")) //Is the root operator ;?
	{
		if (isOperatorString(tree->left->op)) //Is the LEFTside a command?
		{
			pid_t childshell = fork(); //Make a new shell
			if (childshell == 0)
			{
				traverse(tree->left);
				exit(0);
			}
			int status;
			waitpid(childshell, &status, 0);
		}
		else //Leftside IS a command
		{
			pid_t cmd = fork();
			if (cmd == 0)
			{
				execvp(tree->left->cmd.argv[0],tree->left->cmd.argv);
				printf("%s: command not found\n", tree->left->cmd.argv[0]);
				exit(1);
			}
			else
			{
				spawnMonitor(cmd);
			}
			int status;
			waitpid(cmd, &status, 0);
		}
		if (isOperatorString(tree->right->op)) //Is the RIGHTside a command?
		{
			pid_t childshell = fork(); //Make a new shell
			if (childshell == 0)
			{
				traverse(tree->right);
				exit(0);
			}
			int status;
			waitpid(childshell, &status, 0);
		}
		else //Rightside IS a command
		{
			pid_t cmd = fork();
			if (cmd == 0)
			{
				execvp(tree->right->cmd.argv[0],tree->right->cmd.argv);
				printf("%s: command not found\n", tree->right->cmd.argv[0]);
				exit(1);
			}
			else
			{
				spawnMonitor(cmd);
			}
			int status;
			waitpid(cmd, &status, 0);
		}
	}
	else //Neither ; or &, single command entered
	{
		pid_t cmd = fork();
		if (cmd == 0)
		{
			execvp(tree->cmd.argv[0],tree->cmd.argv);
			printf("%s: command not found\n", tree->cmd.argv[0]);
			exit(1);
		}
		else
		{
			spawnMonitor(cmd);
		}
		int status;
		waitpid(cmd, &status, 0);
	}
}

void spawnMonitor(pid_t pid)
{
    if (enableAlarm)
    {
        pid_t monitor = fork();
        if (monitor == 0)
        {
			/*pid_t monitor_monitor = fork();
			if (monitor_monitor)
			{
				sleep(6);
				while (kill(pid, 0) == 0);
				if (kill(monitor_monitor, 0) == 0)
				{
					kill(monitor_monitor, SIGKILL);
					//printf("Killed rogue monitor");
				}
			}
			else {*/
				sleep(5);
				int ret = kill(pid, 0);
				if(ret == 0)
				{
					terminatePrompt(pid);
				}
			//}
            exit(0);
        }
    }
}

void terminatePrompt(pid_t pid)
{
	char answer = ' ';
	printf("Terminate process: %d? (Y/N)", pid);
	fflush(stdout);
	/*
	while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n')
	{
		answer = getc(stdin);
	}
	getchar(); //avoid the case when the next command will be '\r'
	*/
	answer = getchar();
	if (answer == 'Y' || answer == 'y')
	{
		kill(pid, SIGKILL);
		printf("Terminated process: %d.\n", pid);
	}
}