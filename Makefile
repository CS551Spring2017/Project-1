OBJS = profile.c history.c parse-tree.c shell.c
CC = gcc
DEBUG = 
FLAGS = -Wall $(DEBUG)

shell: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o shell

clean:
	\rm shell