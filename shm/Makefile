OBJ1 += shm_read_stdin_0.o
OBJ2 += shm_write_stdout_1.o
read_from_stdin:$(OBJS1)
write_to_stdout:$(OBJ2)
	
%*.o:%*.c
	gcc $^ -o $@ -Wall
.clean:
	rm -rf *.o a.out
