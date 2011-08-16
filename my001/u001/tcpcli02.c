#include "unp.h"
void
str_child(int signo){
  pid_t pid;
  int status;
  
  while((pid = waitpid(-1, &status, WNOHANG)) > 0){
    printf("child %ld terminate\n", pid);
  }
  return;
}

void
str_cli(FILE* fp, int fd){
  char sendline[MAXLINE], recvline[MAXLINE];
  int n, stdineof;
  int maxfd;
  fd_set readfds;
  FD_ZERO(&readfds);
  
  stdineof = 0;
  for(;;){
    if(stdineof == 0){
      FD_SET(fileno(fp), &readfds);
    }
    FD_SET(fd, &readfds);
    maxfd = fileno(fp) > fd ? fileno(fp) : fd;
    if(select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0){
      perror("select");
      exit(1);
    }

    if(FD_ISSET(fileno(fp), &readfds)){
      if(fgets(sendline, sizeof(sendline), fp) != NULL){
	if(write(fd, sendline, strlen(sendline)) <= 0){
	  perror("write");
	  exit(1);
	}  
      }
      
      stdineof = 1;
      shutdown(fd, SHUT_WR);
      FD_CLR(fileno(fp), &readfds);
      
      if(ferror(fp)){
	perror("gets");
	exit(1);
      }
    }
    
    if(FD_ISSET(fd, &readfds)){
      if(( n = read(fd, recvline, sizeof(recvline))) > 0){
	if(write(STDOUT_FILENO, recvline, n) != n){
	  perror("write");
	  exit(1);
	}
      }else{
	if(stdineof == 1){
	  return;
	}else{
	  perror("read server error");
	  exit(1);
	}
	perror("read");
	exit(1);
      }
    }
  }
}


int
main(int argc, char *argv[])
{
  int sockfd, clifd;
  struct sockaddr_in servaddr, cliaddr;
  
  if(argc != 2){
    fprintf(stderr, "usage: ./a.out ip");
    exit(1);
  }
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(1);
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  
  if((clifd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)){
    perror("connect");
    exit(1);
  }

  str_cli(stdin, clifd);
  exit(1);
}
