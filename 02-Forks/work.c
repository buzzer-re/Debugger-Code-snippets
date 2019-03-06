#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>





int main(void) {
	
	printf("Spliting work...\n");


	pid_t pid = fork();


	if (!pid) {
		int a = 0;
		for(int i = 0; i < 100000000; i++ ) {
			a += i*2 + 10 *i;
		}

		return 9;
	} 			

	int status;
	int signal;
	printf("Waiting child finish work...\n");

	waitpid(pid, &status, 0);

	
	if (WIFEXITED(status)) {
		signal = WEXITSTATUS(status); 
		printf("Child exited, status = %s\n", strsignal(signal));
	}
		
	return 0;

}
