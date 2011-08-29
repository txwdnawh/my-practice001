#include "unp.h"
int
connect_nonb(int sockfd, (struct sockaddr *)saptr, socklen_t salen, int nsec){
  int n, flags, error;
  fs_set wset, rset;
  socklen_t len;
  if((flags = fcntl(sockfd, F_GETFL, 0)) < 0){
    perror("fcntl");
    exit(1);
  }
  
  if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0){
    perror("fcntl");
    exit(1);
  }
  
  error = 0;
  
  if((n = connect(sockfd, saptr, salen)) < 0){
    if( errno != EINPROGRESS)
      return (-1);
  }
  
  if(n == 0){
    goto done;
  }
  
  FD_ZERO(&rset);
  FD-SET(sockfd, &rest);
  wset = rset;
  tval.tv_sec = nsec;
  tval.tv_usec = 0;
  
  if((n == select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) == 0){
    close(sockfd);
    errno = ETIMEDOUT;
    return (-1);
  }else if( n < 0){
    perror("select");
    return (-1);
  }
  
  if(FS_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)){
    len = sizeof(error);
    if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
      return (-1);
    }
  }else{
    perror("select");
    exit(1);
  }
  
 done:
  if(fcntl(sockfd, F_SETFL, flags) < 0){
    perror("fcntl");
    exit(1);
  }
  
  if(error){
    close(sockfd);
    errno = error;
    return (-1);
  }
  return 0;
  

}
