#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* total created prior to fork */
int total = 0;

int main(int argc, char* argv[]) {

  /* create child */
  int id = fork();

  /* parent waits for child to complete */
  if(id != 0) {
    wait();
  }

  /* child code */
  if(id == 0) {
    total = 100;
    printf("child: total is %d\n", total);
  }

  /* run by parent process */
  if(id != 0) {
    printf("parent: total is %d\n", total);
    printf("End of simulation\n");
  }

  return 0;
}