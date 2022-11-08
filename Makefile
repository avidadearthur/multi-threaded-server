all: main file_creator

main:main.c
	mkdir -p build
	gcc -Wall -Werror -DSET_MAX_TEMP=25 -o build/datamgr main.c datamgr.c lib/dplist.c

file_creator: file_creator.c
	mkdir -p build
	gcc -Wall -Werror -o build/file_creator file_creator.c

clean:
	rm -r build