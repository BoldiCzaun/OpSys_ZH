#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>

int main(){
	int pipefd[2];
	if(pipe(pipefd) < 0){
		perror(" (!) error opening the pipe\n");
		exit(0);
	}

	int pid = fork();
	if(pid < 0){
		perror(" (!) error forking\n");
		exit(0);
	}
	if(pid == 0){
		// Child
		char* message = "This is a message";
		write(pipefd[1], message, strlen(message) + 1);
		printf(" (C) wrote a message into the pipe\n");
	} else {
		// Parent
		sleep(1);
		char message[32];
		read(pipefd[0], message, 32);
		printf(" (P) read message from pipe: %s\n", message);
		
		wait();
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}