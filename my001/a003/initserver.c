#include "unp.h"

int 
initserver(int type, const struct sockaddr *addr, socklen_t alen)
{
  int sockfd;
  int err = 0;
  int reuse = 1;
  
  if((sockfd = socket(addr->sa_family, type, 0)) < 0){
    return (-1);
  }
  
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
    err = errno;
    goto errout;
  }
  if(bind(sockfd, addr, alen) < 0){
  
    err = errno;
    goto errout;
  }
  
  if(type == SOCK_STREAM || type == SOCK_SEQPACKET){
    if(listen(sockfd, 10) < 0){
      err = errno;
      goto errout;
    }
  }
  
  return (sockfd);
  
 errout:
  close(sockfd);
  errno = err;
  return (-1);
}


