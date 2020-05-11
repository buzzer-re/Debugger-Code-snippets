#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>


#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

#include <unistd.h>



int exists(const char* fpath);
uint64_t add_breakpoint(pid_t pid, uint64_t address);
void handle_breakpoint(pid_t pid);
void display_process_info(pid_t pid);

void dump_memory(pid_t pid);

int main(int argc, char** argv) 
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <bin_name> <breakpoint>\n", argv[0]);
		return 1;
	}
	
	const char* bin_path = argv[1];
	uint64_t breakpoint = strtol(argv[2], NULL, 16);

	if (!exists(bin_path)) {
		fprintf(stderr, "Invalid path!\n");
		return 1;
	} 
	puts("Forking...\n");	
	
	pid_t pid = fork(); 
	
	if (!pid) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(bin_path, bin_path, NULL);	
	}	

	int status = 0;
	int signal;
	wait(&status);
	
	if (WIFEXITED(status)) {
		fprintf(stderr, "Unable to trace %s\n", bin_path);
		return 1;
	}

	printf("Adding breakpoint on 0x%x\n", breakpoint);
	add_breakpoint(pid, breakpoint);

	ptrace(PTRACE_CONT, pid, NULL, NULL);	

	char input[100];
	wait(&status);
	while (!WIFEXITED(status)) {
		signal = WSTOPSIG(status);

		switch(signal) {
			case SIGTRAP:
				puts("We just hit a breakpoint!\n");
				break;
		}
		
		printf("> ");
		fgets(input, 100, stdin);
		if (!strcmp(input, "infor\n")) {
			display_process_info(pid);
		} else if (!strcmp(input, "continue\n")) {
			ptrace(PTRACE_CONT, pid, NULL, NULL);
			wait(&status);
		}

	}
	
	printf("Child %d finished...\n", pid);
	return 0;

}


uint64_t add_breakpoint(pid_t pid, uint64_t address)
{
	uint64_t break_opcode = 0xCC;
	uint64_t code_at = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
	uint64_t breakpoint_code = (code_at & ~0xFF) | break_opcode; 		
	ptrace(PTRACE_POKEDATA, pid, address, breakpoint_code);
	
	return code_at;
}


void handle_breakpoint(pid_t pid)
{
	printf("Child %d hits a breakpoint!\n", pid);

}

int exists(const char* fpath) 
{
	return access(fpath, F_OK) != -1;
}



void display_process_info(pid_t pid) 
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	
	printf("Child %d Registers:\n", pid);
	printf("R15: 0x%x\n", regs.r15);
	printf("R14: 0x%x\n", regs.r14);
	printf("R12: 0x%x\n", regs.r12);
	printf("R11: 0x%x\n", regs.r11);
	printf("R10: 0x%x\n", regs.r10);
	printf("RBP: 0x%x\n", regs.rbp);
	printf("RAX: 0x%x\n", regs.rax);
	printf("RCX: 0x%x\n", regs.rcx);
	printf("RDX: 0x%x\n", regs.rdx);
	printf("RSI: 0x%x\n", regs.rsi);
	printf("RDI: 0x%x\n", regs.rdi);
	printf("RIP: 0x%x\n", regs.rip);
	printf("CS:  0x%x\n", regs.cs);
	printf("EFGLAS: 0x%x\n", regs.eflags);

}

void dump_memory(pid_t pid)
{
    printf("Executando 10 instruções\n");
	pid_t child = pid;
    struct user_regs_struct regs;
	int status;
    for (int i = 0; i < 10; ++i) {
        printf("Passo: %d\n", i+1);
        // Executa uma instrução
        ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
        // Espera sinal do filho
        waitpid(child, &status, 0);
        // Copia o estado atual dos registradores
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
		printf("RIP => 0x%x\n", regs.rip);
    	    // Função local para imprimir os principais registradores		
        puts("\n\n");
    }
}
