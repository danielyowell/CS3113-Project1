// PROOF OF CONCEPT: SHARED MEMORY (the hard way)
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
  #include <sys/types.h>

/* key number */
#define SHMKEY ((key_t) 1497)

/* defines struct shared_mem with int variable "value" */
typedef struct
{
    int value;
} shared_mem;

/* create shared memory (pointer) */
shared_mem *total;

int main(void)
{
    int shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666);
    
    /* main function address */
    char *shmadd;
    shmadd = (char *) 0;
    
    total = (shared_mem*) shmat (shmid, shmadd, 0);
    
    total->value = 25;

    pid_t id = fork();

    /* parent waits for child to complete */
    if(id != 0) {
        wait(NULL);
    }

    /* child process */
    if(id == 0) {
        total->value = 72;
        printf("(child) total is: %d\n", total->value);
    }

    /* if parent process */
    if(id != 0) {
        printf("(parent) total is: %d\n", total->value);
    }

    printf("Process completed\n");
}