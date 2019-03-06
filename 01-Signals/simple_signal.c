#include <stdio.h>
#include <signal.h>
#include <unistd.h> // sleep

void simple_handler(int sig)
{
	printf("Hello SIGINT\n");
}

int main()
{
	struct sigaction sig_handler = { simple_handler };
	
	sigaction(SIGINT, &sig_handler, NULL);

	sleep(1000);
	return 0;
}
