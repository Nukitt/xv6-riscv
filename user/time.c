#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int 
main(int argc, char ** argv) 
{
  int pid = fork();
  if(pid < 0) {
    printf("fork(): failed\n");
    exit(1);
  } else if(pid == 0) {
    if(argc == 1) {
      sleep(1);
      exit(0);
    } else {
      exec(argv[1], argv + 1);
      printf("exec(): failed\n");
      exit(1);
    }  
  } else {
    int r_time, w_time;
    waitx(0, &w_time, &r_time);
    // similkar to wait
    printf("\nwaiting:%d\nrunning:%d\n", w_time, r_time);
  }
  exit(0);
}