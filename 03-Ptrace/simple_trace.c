#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>

#include <sys/types.h>
#include <sys/ptrace.h>



int main(int argc, char* const* argv)
{


	pid_t pid = fork(); //Creating a fork



	if (!pid) {
		printf("Child process!!\n");
		sleep(5);
		short t = ptrace(PTRACE_TRACEME, NULL, NULL, NULL);

		if (!t) {
			printf("I am not being traced!\n");
		} else {
			printf("I am being traced!\n");
		}
		exit(1);
	} 

	printf("Father of %d \n", pid);
	printf("Tracing...\n");
	

//	ptrace(PTRACE_SEIZE, pid, NULL, NULL);
	

	printf("Process traced!\n");

	sleep(7);
	return 0;
}
