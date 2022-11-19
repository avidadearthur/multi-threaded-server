all:main.c sensor_db.c logger.c
	mkdir -p build
	gcc -g -Wall -Werror -o build/main main.c sensor_db.c logger.c

clean:
	rm -r build

zip:
	zip milestone2.zip *.c *.h