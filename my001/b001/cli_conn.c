#include <sys/types.h>        
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define QLEN 10
#define CLI_PATH "/var/tmp"
#define CLI_PERM S_IRWXU

//void main(void){}

int
cli_conn(const char *name){
  int len, err, fd;
  struct sockaddr_un un;
  
  if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    return (-1);
  }
  
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  sprintf(un.sun_path, "%s%05d", CLI_PATH, getpid);
  
  len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  unlink(un.sun_path);
  if(bind(fd, (struct sockaddr *)&un, len) < 0){
    err = errno;
    close(fd);
    errno = err;
    return (-2);
  }
  
  if(chmod(un.sun_path, CLI_PERM) < 0){
    err = errno;
    close(fd);
    errno = err;
    return (-3);
  }
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, name);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  if(connect(fd, (struct sockaddr *)&un, len) < 0){
    err = errno;
    close(fd);
    errno = err;
    return (-4);
  }
  
  return (fd);
}
