#include "unp.h"
int my_open(const char *pathname, int mode);
int
main(int argc, char *argv[])
{
  int fd, n;
  char buf[MAXLINE];
  
  if((fd = my_open(argv[1], O_RDONLY)) < 0){
    fprintf(stderr, "can not open file\n");
    exit(1);
  }
  
  while(( n = read(fd, buf, MAXLINE)) > 0){
    if(write(STDOUT_FILENO, buf, n) < 0){
      perror("write");
      exit(1);
    }
  }
  
  if(n < 0){
    perror("read");
    exit(1);
  }else{
    fprintf(stderr, "end of file");
  }
  exit(0);
}
