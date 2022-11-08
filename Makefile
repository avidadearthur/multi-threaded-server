all: main file_creator

main:main.c
	mkdir -p build
	gcc -Wall -Werror -o build/datamgr main.c

file_creator: file_creator.c
	mkdir -p build
	gcc -Wall -Werror -o build/file_creator file_creator.c

clean:
	rm -r build