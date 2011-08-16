#include "unp.h"

int
main(int argc, char *argv[]){
  int tcpfd, udpfd, maxfd;
  fd_set readset;
  struct sockaddr_in tcpservaddr, udpservaddr;
  
  if((tcpfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("tcpfd");
    exit(1);
  }
  
  memset(&tcpservaddr, 0, sizeof(tcpservaddr));
  tcpservaddr.sin_family = AF_INET;
  tcpservaddr.sin_port = htons(8888);
  tcpservaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(tcpfd, (struct sockaddr *)&tcpservaddr, sizeof(tcpservaddr)) < 0){
    perror("bind");
    exit(1);
  }

  if(listen(tcpfd, 10) < 0){
    perror("listen");
    exit(1);
  }
  
  
  /*---------------------udp-------------------------------*/
  if((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("udpfd");
    exit(1);
  }
  
  memset(&udpservaddr, 0, sizeof(udpservaddr));
  udpservaddr.sin_family = AF_INET;
  udpservaddr.sin_port = htons(8888);
  udpservaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(udpfd, (struct sockaddr *)&udpservaddr, sizeof(udpservaddr)) < 0){
    perror("bind");
    exit(1);
  }
  
  maxfd = tcpfd > udpfd ? tcpfd : udpfd;
  
  FD_ZERO(&readset);
  
  for(;;){
    FD_SET(tcpfd, &readset);
    FD_SET(udpfd, &readset);
    if(select(maxfd + 1, &readset, NULL, NULL, NULL) < 0){
      if(errno == EINTR){
	continue;
      }else{
	perror("slect ");
	exit(1);
      } 	
    }
    
    if(FD_ISSET(tcpfd, &readset)){
      printf("tcp");
    }
    if(FD_ISSET(udpfd, &readset)){
      printf("udp");
    }
  }
  return 0;
}
