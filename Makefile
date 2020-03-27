all: tcp_server tcp_client

tcp_server:
	gcc -Wall -o tcp_server server/main.c && mv tcp_server server/

tcp_client:
	gcc -Wall -o tcp_client client/main.c && mv tcp_client client/

clean:
	rm client/tcp_client server/tcp_server

.PHONY: all clean