#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

int
main(int argc, char **argv)
{
  struct sockaddr_un un;
  int fd, size;
  
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, "foo.socket");
  size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  if(bind(fd, (struct sockaddr *)&un, size ) < 0){
    perror("bind");
    exit(EXIT_FAILURE);
  }
  printf("unix domain socket bind success");
  exit(EXIT_SUCCESS);
}
