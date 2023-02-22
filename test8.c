/* necessary */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

/* ? */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>


/* key number */
#define SHMKEY ((key_t) 1497)

typedef struct
{
    int value;
} shared_mem;

shared_mem *total;

int main(int argc, char* argv[]) {
    int shmid, pid1, pid2, pid3, pid4, ID, status;
    char *shmadd;
    shmadd = (char *) 0;

/* Create and connect to a shared memory segment */    
    shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666);
    total = (shared_mem *) shmat (shmid, shmadd, 0);

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
    
    printf("Program complete\n");
}