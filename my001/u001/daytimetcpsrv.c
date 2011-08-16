#include	"unp.h"


int
main(int argc, char **argv)
{
  int listenfd, connfd, len;
  struct sockaddr_in servaddr, cliaddr;
  char buff[MAXLINE];
  time_t ticks;
  
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(13);	/* daytime server */
  
  if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
    perror("bind");
    exit(1);
  }
  
  if(listen(listenfd, QLEN) < 0){
    perror("listen");
    exit(1);
  }
  
  for ( ; ; ) {
    len = sizeof(cliaddr);
    if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0){
      perror("accept");
      exit(1);
    }
    printf("connection from %s port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    if(write(connfd, buff, strlen(buff)) < 0){
      perror("write");
      exit(1);
    }
    
    close(connfd);
  }
}
