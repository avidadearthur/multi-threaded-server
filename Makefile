all:main.c sbuffer.c
	gcc -g -Wall -Werror -o main main.c sbuffer.c

clean:
	rm -r main

zip:
	zip milestone3.zip *.c *.h