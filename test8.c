#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
  #include <sys/types.h>
  #include <sys/wait.h>

/* key number */
#define SHMKEY ((key_t) 1497)

/* defines struct shared_mem with int variable "value" */
typedef struct
{
    int value;
} shared_mem;

/* create shared memory (pointer) */
shared_mem *total;

/* increases total by some number */
int process1() {
    printf("This is the start of process1\n");
    total->value = 100;
    printf("test\n");
    printf("process1: total = %d\n", total->value);
    return 0;
}

/* MAIN METHOD */
int main(int argc, char* argv[]) {

    /* ID representing shared memory */
    int shmid;
    /* process IDs */
    int pid1, pid2, pid3, pid4;
    /* ??? */
    int ID;
    int status;
    /* main function address */
    char *shmadd;
    shmadd = (char *) 0;

/* Create and connect to a shared memory segment */    
    /* create shared memory */
    shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666);
    /* connect shmid to total at (address shmadd) */
    total = (shared_mem*) shmat (shmid, shmadd, 0);

    /* failure: shmid = -1 */
    if (shmid < 0){
        /* print error */
        perror ("shmget");
        exit (1);
    }
    /* failure: total = -1 */
    shared_mem* failflag = (shared_mem *) -1;
    if (total == failflag) {
        perror ("shmat");
        exit (0);
    }
    
    total->value = 69;
    printf("value of total: %d\n", total->value);

    /* detach */
    int x = shmdt(total);
    /* failure: x = -1 */
    if (x == -1) {
        perror ("shmdt");
        exit (-1);
    }

    /* FORK PROCESS */
    if ((pid1 = fork()) == 0) {
        /* create new address shmadd1 */
        char* shmadd1;
        shmadd1 = (char *) 5;
        /* reattach total at address shmadd1 */
        total = (shared_mem*) shmat (shmid, shmadd1, 0);
        printf("Beginning process1\n");
        int idk = process1();

        /* detach */
        int x = shmdt(total);
        /* failure: x = -1 */
        if (x == -1) {
            perror ("shmdt");
            exit (-1);
        }

        printf("Child with pid %d has just exited.\n", pid1);
    }
    if(pid1 != 0) {
        wait(NULL);
        total = (shared_mem*) shmat (shmid, shmadd, 0);
        printf("Parent function: total = %d (should be 100)\n", total->value);
    }
    printf("Program complete\n");
    return 0;
}