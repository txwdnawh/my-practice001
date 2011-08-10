#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stddef.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
int main()
{
  int fd, size;
  
  struct sockaddr_un un;
  
  un.sun_family = AF_UNIX;
  strncpy(un.sun_path, "foo.socket", 11 );
  
  if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  
  size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  
  if(bind(fd, (struct sockaddr *)&un, size) < 0){
    perror("bind");
    exit(EXIT_FAILURE);
  }
  printf("unix domain socket bound");
}

