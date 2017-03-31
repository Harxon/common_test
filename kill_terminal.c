#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, const char *argv[])
{
	pid_t terminal_pid = -1;
	terminal_pid = getppid();


	kill(terminal_pid, SIGKILL);
	return 0;
}
