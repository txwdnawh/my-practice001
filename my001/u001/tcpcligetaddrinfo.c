#include "unp.h"

int
main(int argc, char *argv[])
{
  int sockfd, len, n;
  char recvline[MAXLINE + 1];
  struct sockaddr *sa;
  char buf[INET_ADDRSTRLEN];

  if(argc != 3){
    fprintf(stderr, "usage: ./a.out ip port");
    exit(1);
  }
  
  sockfd = tcp_connect(argv[1], argv[2]);
  sa = malloc(256);
  len = 256;
  
  if(getpeername(sockfd, sa, &len) < 0){
    perror("getpeername");
    exit(1);
  }

  printf("connect to %s\n", inet_ntop(AF_INET, (struct sockaddr_in *)sa, buf, INET_ADDRSTRLEN ));
  
  while(n = read(sockfd, recvline, MAXLINE) > 0){
    recvline[n] = 0;
    fputs(recvline, stdout);
  }
  if( n < 0){
    perror("read");
    exit(1);
  }
  exit(0);
}
