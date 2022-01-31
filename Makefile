CC=gcc
CFLAGS=-Wall -g

test:
	$(CC) $(CFLAGS) projet2019.c listes_generiques.c test.c C_Project.c

clean:
	rm -rf *.out *.o