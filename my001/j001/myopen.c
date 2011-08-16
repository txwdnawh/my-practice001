#include "unp.h"
int read_fd(int fd1, void *buf, size_t nbytes, int *fd);

int
my_open(const char *pathname, int mode)
{
  int status, sockfd[2], fd;
  pid_t childpid;
  char argsockfd[10], argmode[10], c;
  
  if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) < 0){
    perror("socketpair");
    exit(1);
  }
  
  if((childpid = fork()) < 0){
    perror("fork");
    exit(1);
  }else if(childpid == 0){
    close(sockfd[0]);
    snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
    snprintf(argmode, sizeof(argmode), "%d", mode);
    execl("./openfile", "openfile", argsockfd, pathname, argmode, (char *)NULL);
    perror("execl error");
    exit(1);
  }
  
  close(sockfd[1]);
  if(waitpid(childpid, &status, 0) < 0){
    perror("waitpid");
    exit(1);
  }
  
  if(WIFEXITED(status) == 0){
    perror("child not normal end");
    exit(1);
  }
  
  if(WEXITSTATUS(status) == 0){
    if(read_fd(sockfd[0], &c, 1, &fd) < 0){
      perror("read_fd error");
      exit(1);
    }
  }else{
    fd = -1;
  }
  
  return (fd);
}
