/*
 * CS551 - Project 1 - Shell
 * --
 * Team Members:
 *   John Paul Aldana
 *   Randal Kwok
 *   Sufyan Menk
 */

#define MAX 20
#define MAXARGS 128

typedef struct node Tree;

//Infix to Prefix functions

void infixtoprefix(char infix[MAXARGS],char prefix[MAXARGS]);
void reverse(char array[MAXARGS]);
char pop();
void push(char symbol);
int isOperator(char symbol);
int isNonParenOperator(char symbol);
int isOperatorString(char * symbol);
int prcd(char symbol);
void makeAllCmds(char * prefix);

//Parse tree functions

char *getToken(char **p);
Tree *makeTree(char **p);
void release(Tree *tree);
void printInfix(Tree *tree);

void traverse(Tree *tree);

void resetGlobal();