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
#include <unistd.h>
#include "profile.h"

void loadProfile()
{
	FILE *profile;
	char *pathread;
	size_t bufsize = 100;
	profile = fopen("PROFILE", "r");
	if (profile == NULL)
	{
        perror("PROFILE not found");
		return;
	}
	getline(&pathread, &bufsize, profile);
	printf("PATH set to: %s", pathread);
	putenv(pathread);
	char home[] = "/usr/";
	chdir(home);
}