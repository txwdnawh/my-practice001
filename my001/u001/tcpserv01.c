#include "unp.h"

void sig_child(int signo){
  pid_t pid;
  int status;
  while(1){
  if(( pid = waitpid(-1, &status, WNOHANG)) > 0){
    printf("child %d terminated\n", pid);
  }
  }
  return;
}
void 
str_echo(int clifd)
{
  int arg1, arg2;
  char buf[MAXLINE];
  int n;
  
  for(;;){
    if((n = read(clifd, buf, MAXLINE)) < 0){
      perror("read");
      exit(1);
    }else if(n == 0){
      return ;
    }else{
      if(sscanf(buf, "%ld%ld", &arg1, &arg2) == 2)
	snprintf(buf, sizeof(buf), "%ld\n", arg1 + arg2 );
      else
	snprintf(buf, sizeof(buf), "input error\n");
      
      n = sizeof(buf);
      if(write(clifd, buf, n) < 0){
	perror("write");
	exit(1);
      }
    }
  }
  
}
int
main(int argc, char *argv[])
{
  pid_t pid;
  int sockfd, clifd;
  struct sockaddr_in servaddr, cliaddr;
  socklen_t servlen, clilen;
  char buf[INET_ADDRSTRLEN];
  struct sigaction act;
  
  act.sa_handler = sig_child;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  if(sigaction(SIGCHLD, &act, NULL) < 0){
    perror("sigaction");
    exit(1);
  }
  
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
 
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port   = htons(13);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
    perror("bind");
    exit(1);
  }
  
  if(listen(sockfd, QLEN) < 0){
    perror("listen");
    exit(1);
  }

  for(;;){
    clilen = sizeof(cliaddr);
    if((clifd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0){
      if(errno = EINTR)
	continue;
      
      perror("accept");
      exit(1);
    }
    printf("connection from %s %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buf, INET_ADDRSTRLEN), ntohs(cliaddr.sin_port));
    
    if((pid = fork()) < 0){
      perror("fork");
      exit(1);
    }else if(pid == 0){
      close(sockfd);
      str_echo(clifd);
      close(clifd);
      exit(0);
    }else{
      close(clifd);
    }
  }
}
