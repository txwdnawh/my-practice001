#include <sys/types.h>        
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <errno.h>

#define QLEN 10

int
serv_listen(const char *name)
{
  int unixfd, err;
  struct sockaddr_un un;
  socklen_t len;
  
  if((unixfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    return (-1);
  }
  unlink(name);
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, name);
  len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  
  if(bind(unixfd, (struct sockaddr *)&un, len) < 0){
    err = errno;
    close(unixfd);
    errno = err;
    return -2;
  }
  
  if(listen(unixfd, QLEN) < 0){
    err = errno;
    close(unixfd);
    errno = err;
    return -3;
  }
  
  return(unixfd);
}

void
main(void){}
