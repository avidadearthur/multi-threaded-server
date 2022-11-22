all: test_server

test_server: test_server.c lib/tcpsock.c
	gcc test_server.c lib/tcpsock.c -o test_server

sensor_node: sensor_node.c lib/tcpsock.c
	gcc sensor_node.c lib/tcpsock.c -o sensor_node

runserver: test_server
	./test_server

runclient1: sensor_node
	./sensor_node 1 2 127.0.0.1 $(port)

runclient2: sensor_node
	./sensor_node 2 5 127.0.0.1 $(port)

zip:
	zip plab3.zip *.c *.h

clean:
	rm -r build
