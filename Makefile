OBJS = profile.c history.c parse-tree.c shell.c
TESTOBJS_OUT = hello-world repeater sleeper
CC = gcc
DEBUG = 
FLAGS = -Wall $(DEBUG)

shell: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o shell

clean:
	rm -f shell $(TESTOBJS_OUT)
	
tests:
	$(CC) $(FLAGS) hello-world.c -o hello-world
	$(CC) $(FLAGS) repeater.c -o repeater
	$(CC) $(FLAGS) sleeper.c -o sleeper
	
all: shell tests