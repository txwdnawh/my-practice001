#include "unp.h"

#define CLI_PATH "/var/tmp"
#define CLI_PERM S_IRWXU

int
cli_conn(const char *name)
{
  int sockfd, len, err, rval;
  struct sockaddr_un un;
  
  if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    return (-1);
  }
  
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  snprintf(un.sun_path, strlen(un.sun_path), "%s%05d" CLI_PATH, getpid());
  len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  
  unlink(un.sun_path);
  if(bind(sockfd, (struct sockaddr *)&un, len) < 0){
    rval = -2;
    goto errout;
  }
  
  if(chmod(un.sun_path, CLI_PERM) < 0){
    rval = -3;
    goto errout;
  }
  
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  strncpy(un.sun_path, name, strlen(un.sun_path));
  len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  
  if(connect(sockfd, (struct sockaddr *)&un, len) < 0){
    rval = -4;
    goto errout;
  }
  
  return (sockfd);
  
 errout:
  err = errno;
  close(sockfd);
  errno = err;
  return (rval);
}

void
main(void)
{
}
