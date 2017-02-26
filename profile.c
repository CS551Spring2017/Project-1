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
#include <sys/types.h>
#include <sys/stat.h>
#include "profile.h"

int enableAlarm = 1;

int loadProfile()
{
	FILE *profile;
    char *buffer;
    size_t bufsize = 0;
    profile = fopen("PROFILE", "r");
    if (profile == NULL)
    {
        perror("PROFILE not found.");
        return 1;
    }
    getline(&buffer, &bufsize, profile);
	if (strncmp(buffer, "PATH=", 5))
	{
		perror("Invalid PROFILE file. (First line must be PATH=...)");
		printf("Line read was: '%s'", buffer);
		return 1;
	}
	buffer[strlen(buffer) - 1] = '\0'; // remove \n
	/*
	if (!isDirectory(buffer + 5))
	{
		perror("Invalid PROFILE file. (Invalid PATH)");
		printf("'%s' is not a directory", buffer + 5);
		return 1;
	}
	*/
    printf("PATH set to: %s\n", buffer + 5);
    putenv(buffer + 5);
    getline(&buffer, &bufsize, profile);
	if (strncmp(buffer, "HOME=", 5))
	{
		perror("Invalid PROFILE file. (Second line must be HOME=...)");
		printf("Line read was: '%s'", buffer);
		return 1;
	}
	buffer[strlen(buffer) - 1] = '\0'; // remove \n
	if (!isDirectory(buffer + 5))
	{
		perror("Invalid PROFILE file. (Invalid HOME)");
		printf("'%s' is not a directory", buffer + 5);
		return 1;
	}
    printf("HOME set to: %s\n",buffer + 5);
    chdir(buffer + 5);
	if (strncmp(buffer, "ALARM=", 6))
    {
        if(strncmp(buffer + 6, "1", 1))
        {
			printf("Alarm is set to: enabled.\n");
            return 1;
        }
        else
        {
			printf("Alarm is set to: disabled.\n");
            return 0;
        }
    }
    else
    {
        perror("Invalid PROFILE file. (ALARM setting not found, defaulting to ON)");
        return 1;
    }
    fclose(profile);
}

int isDirectory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}