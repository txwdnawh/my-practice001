#include "unp.h"

int
main(int argc, char **argv){
  int fd;
  ssize_t n;
  printf("enter openfile\n");
  if(argc != 4){
    perror("argc != 4");
    exit(1);
  }

  if((fd = open(argv[2], atoi(argv[3]))) < 0){
    perror("open");
    exit(1);
  }
  printf("write fd  fd = %d\n", fd);
  if((n = write_fd(atoi(argv[1]), "", 1, fd)) < 0){
    perror("write fd error");
    exit(1);
  }
  
  exit(0);
}
