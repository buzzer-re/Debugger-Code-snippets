#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Log errors
void fatal(const char* err_msg)
{
	fprintf(stderr, "Error: %s\n", err_msg);
}

// Escreve algo random em um arquivo
void random_work() 
{
	FILE* temp_files = fopen("/tmp/foo", "w");
	
  	if (!temp_files) {
      fatal("Cant open foo!");
    } else {
      fprintf(temp_files, "%s", "Random random random!\n");
      fclose(temp_files);
    }
}

// Handler para deleta arquivos criados
void handler_termination(int sig)
{
	// Verifica se existe usando a function access
	// Caso existe usa a syscall unlink para remover o arquivo
	if (access("/tmp/foo", R_OK) < 0) return;
	
  	unlink("/tmp/foo");
	printf("All clean! closing...\n");
}

int main() {
	//struct sigaction que recebe a function handler_termination como valor do seu handler
	struct sigaction interruption_handler;
	interruption_handler.sa_handler = handler_termination;
	
	// Syscall sigaction que associa o nosso handler para um sinal especifico 
	// O ultimo campo NULL, espera o handler anterior para que posso tornar o novo handler o default	
	sigaction(SIGINT, &interruption_handler, NULL);
	random_work();

	sleep(1000);	
	handler_termination(0);
	return 0;
}
