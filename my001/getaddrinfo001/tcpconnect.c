#include "unp.h"

int
tcp_connect(const char *host, const char *serv)
{
  struct addrinfo hints, *res, *aip, *ressave;
  int sockfd, err, myerr;
  char buf[INET_ADDRSTRLEN];
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  
  if((err = getaddrinfo(host, serv, &hints, &res)) != 0){
    printf("%s\n", gai_strerror(err));
    exit(1);
  }
  ressave = res;
  for(aip = res ; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0)
      continue;
    
    if((connect(sockfd, aip->ai_addr, aip->ai_addrlen)) == 0){
      printf("connect to %s \n", inet_ntop(AF_INET, &((struct sockaddr_in *)aip->ai_addr)->sin_addr, buf, INET_ADDRSTRLEN));
      break;
    }
    
    myerr = errno;
    close(sockfd);
    errno = myerr;
  } 
  if(res == NULL){
    fprintf(stderr, "tcp connect error for %s %s", host, serv);
    exit(1);
  }
  freeaddrinfo(ressave);
  return (sockfd);
}
/*
void
main(void){}
*/
