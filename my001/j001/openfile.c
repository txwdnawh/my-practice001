#include "unp.h"
int write_fd(int fd, void *buf,size_t nbytes, int sendfd);

int
main(int argc, char *argv[])
{
  int fd;
  if(argc != 4){
    perror("argc != 4");
    exit(1);
  }
  
  if((fd = open(argv[2], atoi(argv[3]))) < 0){
    perror("open");
    exit(1);
  }
  
  if(write_fd(atoi(argv[1]), "", 1, fd) < 0){
    perror("write_fd error");
    exit(1);
  }
  exit(0);
}
