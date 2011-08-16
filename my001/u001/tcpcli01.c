#include "unp.h"
void
str_cli(FILE *fin, int fd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  int n;
  
  while(fgets(sendline, MAXLINE, fin) != NULL){
    write(fd, "aaa", 3);
    
    if(write(fd, sendline, sizeof(sendline)) < 0){
      perror("write");
      exit(1);
    }
    
    if(( n = read(fd, recvline, MAXLINE)) > 0){
      fputs( recvline, stdout);
    }else{
      perror("read");
      exit(1);
    }
  }
}
int
main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr;
  
  if(argc != 2){
    fprintf(stderr, "usage: ./a.out ip");
    exit(1);
  }
  
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  
  if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
    perror("connect");
    exit(1);
  }
  
  str_cli(stdin, sockfd);
  exit(0);
}
