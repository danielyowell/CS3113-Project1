// PROJECT 1
// Written by: Daniel Yowell
// @danielyowell

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

void process1() {
    while(total->value < 100000) {
        total->value = total->value + 1;
    }
    printf("From Process 1: counter = %d.\n", total->value);
    return;
}
void process2() {
    while(total->value < 200000) {
        total->value = total->value + 1;
    }
    printf("From Process 2: counter = %d.\n", total->value);
    return;
}
void process3() {
    while(total->value < 300000) {
        total->value = total->value + 1;
    }
    printf("From Process 3: counter = %d.\n", total->value);
    return;
}
void process4() {
    while(total->value < 500000) {
        total->value = total->value + 1;
    }
    printf("From Process 4: counter = %d.\n", total->value);
    return;
}

int main(void)
{
    int shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666);
    
    /* main function address */
    char *shmadd;
    shmadd = (char *) 0;
    
    total = (shared_mem*) shmat (shmid, shmadd, 0);
    
    total->value = 0;
    /* printf("current value: %d\n", total->value); */

    pid_t id;
    int cpid;

    id = fork();
    // CHILD 1
    if(id == 0) {
        process1();
    }
    // PARENT
    if(id != 0) {
        cpid = wait(NULL);
        printf("Child with ID: %d has just exited.\n", cpid);
        id = fork();
        // CHILD 2
        if(id == 0) {
            process2();
        }
        // PARENT
        if(id != 0) {
            cpid = wait(NULL);
            printf("Child with ID: %d has just exited.\n", cpid);
            id = fork();
            // CHILD 3
            if(id == 0) {
                process3();
            }
            // PARENT
            if(id != 0) {
                cpid = wait(NULL);
                printf("Child with ID: %d has just exited.\n", cpid);
                id = fork();
                // CHILD 4
                if(id == 0) {
                    process4();
                }
                // PARENT
                if(id != 0) {
                    cpid = wait(NULL);
                    printf("Child with ID: %d has just exited.\n", cpid);
                    
                    // detach shared memory
                    if (shmdt(total) == -1) {
                        perror ("shmdt");
                        exit (-1);
                    }   
                    // delete shared memory
                    shmctl(shmid, IPC_RMID, NULL); 

                    printf("\nEnd of simulation.\n");
                }
            }
        }
    }
}