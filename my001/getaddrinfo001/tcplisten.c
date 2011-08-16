#include "unp.h"

int 
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
  int listenfd, n, err;
  const int on = 1;
  struct addrinfo hints, *ailist, *aisave, *aip;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  
  if((err = getaddrinfo(host, serv, &hints, &ailist)) != 0){
    fprintf(stderr, "%s\n", gai_strerror(err));
    exit(1);
  }

  aisave = ailist;
  for(aip = ailist; aip != NULL; aip = aip->ai_next){
    if((listenfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0){
      continue;
    }
    
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
      perror("setsockopt");
      exit(1);
    }
    
    if(bind(listenfd, aip->ai_addr, aip->ai_addrlen) == 0){
      break;
    }else{
      perror("bind");
      exit(1);
    }
    
    close(listenfd);
  }
  
  if(aip == NULL){
    fprintf(stderr, "bind error to %s %s\n", host, serv);
    exit(1);
  }
  
  if(listen(listenfd, 10) != 0){
    perror("listen");
    exit(1);
  }
  
  if(addrlenp != NULL)
    *addrlenp = aip->ai_addrlen;
  
  freeaddrinfo(ailist);
  return (listenfd);
}

//void main(void){}
