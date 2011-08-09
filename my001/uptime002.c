#include <syslog.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#define QLEN 10

int
initserver(struct addrinfo *aip){
  int sockfd;
  
  if((sockfd = socket(aip->ai_family, aip->ai_socktype, 0)) < 0){
      perror("getaddrinfo");
      return (-1);
    }
    
    if(bind(sockfd, aip->ai_addr, aip->ai_addrlen) < 0){
      perror("bind");
      return (-1);
    }
    
    if(listen(sockfd, QLEN) < 0){
      perror("listen");
      return (-1);
    }
    return sockfd;
}
void
serve(int sockfd)
{
  int clifd,status;
  pid_t pid;
  
  for(;;){
    clifd = accept(sockfd, NULL, NULL);
    if(clifd < 0){
      syslog(LOG_ERR, "ruptimed: accept errpor: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }
    if((pid = fork()) < 0){
      syslog(LOG_ERR, "ruptimed: fork error: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }else if(pid == 0){
      if(dup2(clifd, STDOUT_FILENO) != STDOUT_FILENO ||
	 dup2(clifd, STDERR_FILENO) != STDERR_FILENO){
	syslog(LOG_ERR, "ruptimed: dup2 error: %s", strerror(errno));
	exit(EXIT_FAILURE);
      }
      close(clifd);
      execl("/usr/bin/uptime", "uptime", (char *)0);
      syslog(LOG_ERR, "ruptimed: exec error: %s", strerror(errno));
      exit(EXIT_FAILURE);
    }else{
      close(clifd);
      waitpid(pid, &status, 0);
    }
  }
}

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
    if((sockfd = initserver(aip)) > 0){
      serve(sockfd);
      exit(0);
    }
    
    
  }
  

  exit(1);
}
