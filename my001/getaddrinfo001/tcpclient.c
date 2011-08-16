#include "unp.h"
int tcp_connect(const char *host, const char *serv);

int
main(int argc, char *argv[])
{
  int n, connfd;
  char sendline[MAXLINE];
  
  if(argc != 3){
    fprintf(stderr, "usage: ./client ip port");
    exit(1);
  }
  
  connfd = tcp_connect(argv[1], argv[2]);
    
  for(;;){
    if((n = read(connfd, sendline, MAXLINE)) < 0){
      perror("read");
      exit(1);
    }
    sendline[n] = 0;
    fputs(sendline, stdout);
  }
  
  exit(1);
}
