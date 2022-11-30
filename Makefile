all:main.c sbuffer.c
	mkdir -p build
	gcc -g -Wall -Werror -o build/main main.c sbuffer.c

clean:
	rm -r build

zip:
	zip milestone3.zip *.c *.h