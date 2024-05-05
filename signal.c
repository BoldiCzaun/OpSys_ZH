#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<signal.h>

void sig_handler(){
	return;
}

int main(){
	int pid;
	signal(SIGUSR1, sig_handler);
	pid = fork();
	if(pid < 0){
		perror(" (!) error forking\n");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		// Child
		sleep(1);
		kill(getppid(), SIGUSR1);
		printf(" (C) sent signal to parent\n");
		printf(" (C) waiting to a signal from parent\n");
		pause();
		printf(" (C) signal received from parent\n (C) terminating...\n");
		exit(EXIT_SUCCESS);
	} else {
		// parent
		printf(" (P) waiting to a signal from child...\n");
		pause();
		printf(" (P) signal received from child\n");
		printf(" (P) sending a signal to child\n");
		kill(pid, SIGUSR1);
		printf(" (P) waiting for child to finish\n");
		wait();
		printf(" (P) terminating...\n");
		exit(EXIT_SUCCESS);
	}
	return 0;
}