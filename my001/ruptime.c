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
  
  if((err = getaddrinfo(argv[1], "8888", &hint, &res )) != 0){
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  for(aip = res; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0){
      perror("getaddrinfo");
      exit(EXIT_FAILURE);
    }
    if(connect(sockfd, aip->ai_addr, aip->ai_addrlen ) < 0){
      perror("connect");
      exit(EXIT_FAILURE);
    }else{
      while((n = recv(sockfd, buf, 256, 0)) > 0){
	write(STDOUT_FILENO, buf, n);
      }
      
      if(n < 0){
	perror("recv");
	exit(EXIT_FAILURE);
      }
    }
  }

  exit(0);
}
