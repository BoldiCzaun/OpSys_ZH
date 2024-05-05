#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define MAX_MSG_SIZE 64 

// use -lrt -pthread flag with gcc 

int main(int argc, char** argv) {
	int pid;

	// egyedi kulcs generalasa
	key_t kulcs = ftok(argv[0], 1);
	// osztott memoria letrehozasa MAX_MSG_SIZE merettel
	int shm_id = shmget(kulcs, MAX_MSG_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
	// attaches memory to msg 
	char* msg = shmat(shm_id, NULL, 0);

	pid = fork();
	if(pid < 0){
		perror(" (!) error forking\n");
		exit(EXIT_FAILURE);
	}
	if(pid == 0){
		// child
		sem_t* sem = sem_open("/sem", O_CREAT, 0644, 0);
		printf(" (C) Waiting for semaphore to open\n");
		sem_wait(sem);
		printf(" (C) semaphore opened\n");
		printf(" (C) Reader received: %s\n", msg);
		sem_close(sem);
		shmdt(msg);

		printf(" (C) terminating...\n");
		exit(EXIT_SUCCESS);
	} else {
		// parent
		sem_t* sem = sem_open("/sem", O_CREAT, 0644, 0);

		char* buffer = "The message";
		strcpy(msg, buffer);
		printf(" (P) writing into memory\n");

		// detaches memory from msg
		shmdt(msg);
		printf(" (P) opening semaphore\n");
		sem_post(sem);
		wait();

		shmctl(shm_id, IPC_RMID, NULL);
		sem_close(sem);
		sem_unlink("/sem");
		printf(" (P) terminating...\n");
		exit(EXIT_SUCCESS);
	}
	return 0;
}