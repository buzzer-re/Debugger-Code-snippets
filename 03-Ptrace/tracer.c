#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>



void display_regs(struct user_regs_struct* regs)
{
	printf("RIP: 0x%x\n", regs->rip);
	printf("RBP: 0x%x\n", regs->rbp);
	printf("RSP: 0x%x\n", regs->rsp);
}

int main(int argc, char* const* argv)
{

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <program_path>\n", argv[0]);
		return 1;
	}

	const char* progName = argv[1];
	
	pid_t child = fork();

	if (!child) {
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execl(progName, progName, NULL);
	}
	
	int status;
	int options = 0;
	int signal;

	// Estrutura que mantem os registradores
	struct user_regs_struct regs;

	/// Capta primeiro sinal de parada do filho
	waitpid(child, &status, 0);
	signal = WSTOPSIG(status);

	if (signal == SIGTRAP) {
		printf("Processo alvo %s esperando pronto para iniciar\n\n", progName);
	}
	
	printf("Executando 10 instruções\n");
	for (int i = 0; i < 10; ++i) {
		printf("Passo: %d\n", i+1);
		// Executa uma instrução
		ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
		// Espera sinal do filho
		waitpid(child, &status, 0);
		// Copia o estado atual dos registradores
		ptrace(PTRACE_GETREGS, child, NULL, &regs);

		// Função local para imprimir os principais registradores
		display_regs(&regs);
		puts("\n\n");
	}

	puts("Continuando...\n");

	/// Continua execução
	ptrace(PTRACE_CONT, child, NULL, NULL);
	waitpid(child, &status, 0);

	printf("Filho saiu com %d\n", WIFEXITED(status));
	return 0;
}
