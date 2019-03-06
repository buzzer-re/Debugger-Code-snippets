#include <stdio.h>

// Acesso a syscall
#include <unistd.h>


// Acesso ao tipo variavel pid_t
#include <sys/types.h>


int main(void)
{
	int x;

	printf("Processo normal...\n");
	printf("Forking...\n");
	sleep(1);
	
	pid_t pid = fork();
	x = 40;

	if (!pid) {
		printf("Eu sou o processo filho meu PID: %d\n", pid);
	} 
	
	else {	
		printf("Eu sou o processo pai de %d\n", pid);
	}

	
	sleep(10);
	return 0;
}
