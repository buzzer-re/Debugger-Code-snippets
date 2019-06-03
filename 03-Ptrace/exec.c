#include <unistd.h>
#include <stdio.h>


int main(int argc, char* const* argv)
{

	
	if (argc < 3) {
		printf("Usage: %s <command> <args>\n", argv[0]);
		return 1;
	}

	
	const char* command = argv[1];
	
	char* const* args = &argv[1];
	

	printf("First arg => %s\n", args[0]);

	execv(command,  args);
	

	puts("Continua?\n");	

	return 0;
}
