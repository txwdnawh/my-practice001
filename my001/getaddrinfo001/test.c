#include "unp.h"
#include <sys/un.h>
#include <stddef.h>
int
main(int argc, char *argv[])
{
  int sockfd;
  socklen_t len;
  struct sockaddr_un addr1, addr2;
  
  if(argc != 2){
    fprintf(stderr, "usage: ./a.out name");
    exit(1);
  }

  if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  
  unlink(argv[1]);
  
  memset(&addr1, 0, sizeof(addr1));
  addr1.sun_family = AF_UNIX;
  strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) -1);
  
  len = offsetof(struct sockaddr_un, sun_path) + strlen(addr1.sun_path);
  bind(sockfd, (struct sockaddr *)&addr1, len);
  
  len = sizeof(addr2);
  getsockname(sockfd, (struct sockaddr *)&addr2, &len);
  printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);
  exit(0);
  
}

