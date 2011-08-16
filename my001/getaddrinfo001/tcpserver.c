#include "unp.h"
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
void daemon_init(const char *, int);
int
main(int argc, char *argv[])
{
  time_t ticks;
  char buf[INET_ADDRSTRLEN];
  int listenfd, connfd, n;
  char sendline[MAXLINE];
  socklen_t len, addrlen;
  struct sockaddr *cliaddr;
  struct sockaddr_in *addr;
  daemon_init(argv[0], 0);
  int a = 100;
  if(argc != 2){
    fprintf(stderr, "usage: ./a.out ip port\n");
    exit(1);
  }
  listenfd = tcp_listen(NULL, argv[1], &len);
  
  if(listenfd < 0){
    perror("tcp_listen");
    exit(1);
  }
  
  if((cliaddr = malloc(len)) == NULL){
    perror("malloc");
    exit(1);
  }
  
  for(;;){
    addrlen = len;
    connfd = accept(listenfd, cliaddr, &addrlen);
    if(connfd < 0){
      perror("accept");
      exit(1);
    }
    addr = (struct sockaddr_in *)cliaddr;
    printf("connectin from %s %d\n", inet_ntop(AF_INET, &addr->sin_addr, buf, INET_ADDRSTRLEN), ntohs(addr->sin_port));
    
    ticks = time(NULL);
    snprintf(sendline, sizeof(sendline), "%.24s\r\n", ctime(&ticks));
    write(connfd, sendline, strlen(sendline));
    close(connfd);
  }
}
