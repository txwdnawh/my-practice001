#include "unp.h"
#define MAXLINE 256

int
main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;
  
  if (argc != 2){
    fprintf(stderr, "usage: a.out <IPaddress>");
    exit(1);
  }
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket error");
    exit(1);
  }
  
  memset(&servaddr,0,  sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port   = htons(13);	/* daytime server */
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
    perror("inet_pton error ");
    exit(1);
  }
  
  if (connect(sockfd, (struct sockaddr  *) &servaddr, sizeof(servaddr)) < 0){
    perror("connect error");
    exit(1);
  }
  
  while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;	/* null terminate */
    if (fputs(recvline, stdout) == EOF){
      perror("fputs error");
      exit(1);
    }
  }
  if (n < 0){
    perror("read error");
    exit(1);
  }
  
  exit(0);
}


