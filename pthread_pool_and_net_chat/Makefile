all:client server
client:client.o
server:server.o tpool.o
	gcc -g $^ -o $@ -lpthread -O3

.PHONY:clean
clean:
	rm -rf *.o client server
