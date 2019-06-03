#include <stdio.h>

#include <sys/types.h>
#include <sys/ptrace.h>

#include <unistd.h>



int main(int argc, char* const* argv)
{

	
	if (argc < 3) {
		printf("Usage: %s <command> <args>\n", argv[0]);
		return 1;
	}

	
	const char* command = argv[1];
	
	char* const* args = &argv[1];
	

	pid_t child_pid = fork();

	// Neste ponto, todas as variaveis sao copiadas para o nosso fork
	// o fork NAO recebe as mesmas variaveis, apenas uma copia ;)	
	if (!child_pid) {
		// Hora de transformar nosso fork em outro programa
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execv(command, args);
	}	
	
	
	char in;
	do {
		puts("Iniciar processo ? [y/n]: ");
		in = getchar();
	} while (in != 'y');

	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	
	return 0;
}
