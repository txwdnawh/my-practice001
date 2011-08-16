#include "unp.h"
static int count = 0;
void
sig_int(int signo){
  printf("recv count: %d\n", count);
}
void 
udp_echo(int sockfd, struct sockaddr * cliaddr, socklen_t clilen){
  
  int n, len;
  char mesg[MAXLINE];
  char buf[INET_ADDRSTRLEN];
  struct sockaddr_in *addr;
  struct sigaction act;
  
  act.sa_handler = sig_int;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if(sigaction(SIGINT, &act, NULL) < 0){
    perror("sigaction");
    exit(1);
  }
  
  
  /*  for(;;){
    len = clilen;
    if((n = recvfrom(sockfd, mesg, MAXLINE, 0, cliaddr, &clilen  )) < 0){
      perror("recvfrom");
      exit(1);
    }
    printf("connection from : %d\n", n);
    addr = (struct sockaddr_in *)cliaddr;
    printf("connection from %s %d\n", inet_ntop(AF_INET,&(addr->sin_addr),  buf, INET_ADDRSTRLEN), ntohs(addr->sin_port));
    if(sendto(sockfd, mesg, n, 0, cliaddr, clilen) < 0){
      perror("sendto");
      exit(1);
    }
    count +=1;
    }*/
  while(recvfrom(sockfd, mesg, MAXLINE, 0, cliaddr, &clilen  ) > 0){
    count += 1;
  }
}
int
main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, cliaddr;
  int sockfd, len;
  
 
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
    perror("bind");
    exit(1);
  }
  
  len = sizeof(cliaddr);
  udp_echo(sockfd, (struct sockaddr *)&cliaddr, len);
  exit(0);
}
