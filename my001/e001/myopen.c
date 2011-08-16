#include "unp.h"

int
my_open(const char *pathname, int mode)
{
  printf("enter myopen\n");
  int sockfd[2], fd, status;
  pid_t pid;
  char argsockfd[10], argmode[10], c;
  if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) < 0){
    perror("socketpair");
    exit(1);
  }
  
  if((pid = fork()) < 0){
    perror("fork");
    exit(1);
  }else if(pid == 0){
    close(sockfd[0]);
    snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
    snprintf(argmode, sizeof(argmode), "%d", mode);
    printf("excecl openfile\n");
    execl("./openfile", "openfile", argsockfd, pathname, argmode, (char *)NULL);
    perror("execl");
    exit(1);
  }
  close(sockfd[1]);
  
  if(waitpid(pid, &status, 0) < 0){
    perror("waitpid");
    exit(1);
  }
  if(WIFEXITED(status) == 0){
    perror("child error");
    exit(1);
  }
  if((status = WEXITSTATUS(status)) == 0){ 
    printf("read fd\n");
    read_fd(sockfd[0], &c, 1, &fd);
    printf("bbbbbbbbbbbbbbbbbbbbbbbb%d\n", fd);
  }else{
    errno = status;
    fd = -1;
  }
  
  close(sockfd[0]);
  printf("return fd\n");
  return (fd);
}
