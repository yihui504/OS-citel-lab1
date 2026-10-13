osh: shell.c
	gcc -Wall -Wextra -O2 -std=gnu11 -o osh shell.c

clean:
	rm -f osh

.PHONY: clean
