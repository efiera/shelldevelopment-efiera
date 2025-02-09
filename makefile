main: resolve.c main.c commands.c internal.c helpers.c
	gcc resolve.c main.c commands.c internal.c helpers.c -o shell -Wall -Werror


clean:
	rm -rf *.o shell.out