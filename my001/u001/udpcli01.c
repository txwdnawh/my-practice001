#include "unp.h"
void 
udp_cli(FILE *fp, int sockfd, struct sockaddr *servaddr, socklen_t len){
  int n;
  char sendline[MAXLINE], recvline[MAXLINE];
  struct sockaddr * reply_addr;
  char buf[1400];
  int i;
  for(i = 0; i< 2000; i++){
    printf("%d,", i);
    if(i%10 == 0){
      printf("\n");
    }
    if(sendto(sockfd, buf, 1400, 0, servaddr, len) < 0){
      perror("sendto");
      exit(1);
    }
  }
  
  /*  while(fgets(sendline, MAXLINE, fp) != NULL){
    
    if(sendto(sockfd, sendline, strlen(sendline), 0, servaddr, len) < 0){
      
      perror("sendto aaa");
      
      exit(1);
    }
    
    if((n = recvfrom(sockfd, recvline, MAXLINE, 0,  reply_addr , &len )) < 0){
      perror("recvform");
      exit(1);
    }
    recvline[n] = 0;
    
    fputs(recvline, stdout);
  }
  
  if(ferror(fp))
    perror("fgets");
  */
}
int
main(int argc, char *argv[])
{
  struct sockaddr_in servaddr;
  int sockfd, len;
  
  if(argc != 2){
    fprintf(stderr, "usage: ./a.out ip");
    exit(1);
  }

  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  memset(&servaddr, 0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
 
  
  len = sizeof(servaddr);
  udp_cli(stdin, sockfd, (struct sockaddr *)&servaddr, len);
  exit(0);
}
