#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
int
main(int argc, char *argv[]){
  struct addrinfo hint;
  struct addrinfo *res, *aip;
  int err;
  int sockfd;
  int n;
  char buf[256];
  int clifd;
  if(argc != 2){
    fprintf(stderr, "Usage: ruptime servername");
    exit(EXIT_FAILURE);
  }
  
  hint.ai_flags = 0;
  hint.ai_family = 0;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_addrlen = 0;
  hint.ai_addr = NULL;
  hint.ai_canonname = NULL;
  hint.ai_next = NULL;
  
  if((err = getaddrinfo("127.0.0.1", "8888", &hint, &res )) != 0){
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  for(aip = res; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0){
      perror("getaddrinfo");
      exit(EXIT_FAILURE);
    }
    
    if(bind(sockfd, aip->ai_addr, aip->ai_addrlen) < 0){
      perror("bind");
      exit(1);
    }
    
    if(listen(sockfd, 10) < 0){
      perror("listen");
      exit(1);
    }

    for(;;){
      clifd = accept(sockfd, NULL, NULL);
      
      if(clifd < 0){
	perror("accept");
	exit(1);
      }
      
      if(write(clifd, "hello world", 11 ) != 11){
	perror("write cli");
	exit(1);
      }
    }
    
  }
  

  exit(0);
}
