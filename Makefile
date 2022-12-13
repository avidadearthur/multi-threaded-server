all: main.c datamgr.c lib/dplist.c
	mkdir -p build
	gcc -g -Wall -Werror -D SET_MAX_TEMP=25 -D SET_MIN_TEMP=0 -o build/datamgr main.c datamgr.c lib/dplist.c

main:main.c datamgr.c lib/dplist.c
	mkdir -p build
	gcc -g -Wall -Werror -D SET_MAX_TEMP=25 -D SET_MIN_TEMP=0 -o build/datamgr main.c datamgr.c lib/dplist.c

main_debug:main.c datamgr.c lib/dplist.c
	mkdir -p build
	gcc -g -Wall -Werror -D SET_MAX_TEMP=25 -D SET_MIN_TEMP=0 -o build/datamgr main.c datamgr.c lib/dplist.c

file_creator: file_creator.c
	mkdir -p build
	gcc -Wall -Werror -o build/file_creator file_creator.c

clean:
	rm -r build