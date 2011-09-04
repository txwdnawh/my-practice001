#include "unp.h"

int
serv_listen(const char *name)
{
  int err, sockfd, olen, len, size, rval;
  struct sockaddr_un un;
  
  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  olen = sizeof(un.sun_path);
  len = strlen(name);
  if(len > olen)
    return (-1);
  strncpy(un.sun_path, name, strlen(name));
  size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  
  if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    return (-2);
  }
  unlink(name);
  
  if(bind(sockfd, (struct sockaddr *)&un, size ) < 0){
    rval = (-3);
    goto errout;
  }
  
  if(listen(sockfd, QLEN) < 0){
    rval = (-4);
    goto errout;
  }
  
  return (sockfd);
 
 errout:
  err = errno;
  close(sockfd);
  errno = err;
  return (rval);
}

/*
int
main(int argc, char *argv[])
{
  

  exit(0);
}


*/
 /*
char *
strncpy(char *dest, const char *src, size_t n)
{
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dest[i] = src[i];
  for ( ; i < n; i++)
    dest[i] = '\0';
	       
  return dest;
}


char *
strncpy(char *dest, const char *src, size_t n)
{
  size_t i;
  
  for(i = 0; i < n && src[i] != '\0'; i++){
    dest[i] = src[i];
  }
  
  for(; i < n; i++){
    dest[i] = '\0';
  }
  return dest;
}
 */
