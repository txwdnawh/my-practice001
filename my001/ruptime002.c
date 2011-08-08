#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#define BUFLEN 256
extern int connect_retry(int sockfd, struct sockaddr *addr, socklen_t len);
void print_uptime(int sockfd);

int
main(int argc, char *argv[]){
  struct addrinfo hint;
  struct addrinfo *res, *aip;
  int err;
  int sockfd;

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
    err = errno;
  }

  for(aip = res; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0){
      err = errno;
    }
    if(connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen ) < 0){
      perror("connect_retry");
      exit(EXIT_FAILURE);
    }else{
      print_uptime(sockfd);
      exit(EXIT_SUCCESS);
    }

    
  }
  fprintf(stderr, "can not connect to %s: %s\n", argv[1], strerror(err));
  
  exit(EXIT_FAILURE);
}

void
print_uptime(int sockfd)
{
  char buf[BUFLEN];
  int n;
  while((n = recv(sockfd, buf, BUFLEN, 0 )) > 0){
    
    if(write(STDOUT_FILENO, buf, n) != n){
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  if( n < 0){
    perror("recv");
    exit(EXIT_FAILURE);
  }
}
