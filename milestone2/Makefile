all:main.c sensor_db.c logger.c
	gcc -g -Wall -Werror -o main main.c sensor_db.c logger.c

clean:
	rm -r *.csv gateway.log main

zip:
	zip milestone2.zip *.c *.h