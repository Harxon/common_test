all:app
app:main.o pthread_pool.o
	gcc -g $^ -o  $@ -lpthread -O3
clean:
	rm -rf *.o $(all)
