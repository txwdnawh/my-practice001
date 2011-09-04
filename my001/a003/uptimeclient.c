#include "unp.h"
#define MAXADDRLEN 256

extern int connect_retry(int fd, const struct sockaddr *addr, socklen_t alen);

void
print_uptime(int sockfd)
{
  int n;
  char buf[256];
  
  while((n = recv(sockfd, buf, 256, 0)) > 0){
    if(write(STDOUT_FILENO, buf, n) < 0){
      err_quit("write");
    }
  }
}

int
main(int argc, char *argv[])
{
  struct addrinfo *ailist, *aip;
  struct addrinfo hint;
  int sockfd, err;
  
  hint.ai_flags = 0;
  hint.ai_family = 0;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_addrlen = 0;
  hint.ai_canonname =NULL;
  hint.ai_addr = NULL;
  hint.ai_next = NULL;
  
  if((err = getaddrinfo("127.0.0.1", "9999", &hint, &ailist)) != 0){
    err_quit("getaddrinfo");
  }
  
  for(aip = ailist; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family, SOCK_STREAM, 0)) < 0){
      err = errno;
    }
    if(connect_retry(sockfd, aip->ai_addr, aip->ai_addrlen) < 0){
      err = errno;
    }else{
      print_uptime(sockfd);
      exit(0);
    }
  }
  
  fprintf(stderr, "can't connect to %s: %s\n", argv[1], strerror(err));
  exit(1);
}


