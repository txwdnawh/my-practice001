#include "unp.h"
static ssize_t
my_read(int fd, char *ptr){
  static int
}
ssize_t
readline(int fd, void* buf, size_t maxlen){
  int n, rc; 
  char *ptr;
  char c;
  ptr = buf;
  for(n = 1; n < maxlen; n++){
  AGAIN:
    if((rc = read(fd, &c, 1)) == 1){
      *ptr++ = c;
      if(c == '\n')
	break;
    }else if(rc == 0){
      if(n == 1)
	return 0;
      else 
	break;
    }else{
      if(errno == EINTR)
	goto AGAIN;
      return -1;
    }
    
  }
  *ptr = 0;
  return n;
}
ssize_t 
readn(int fd, void *buf, size_t len){
  size_t nread, nleft;
  char *ptr;
  ptr = buf;
  nleft = len;
  while(nleft > 0){
    if((nread = read(fd, ptr, nleft)) < 0){
      if(errno = EINTR)
	nread = 0;
      else
	return -1;
 
    }else if(nread == 0){
      break;
    }
    nleft -=nread;
    prt += nread;
  }
  return len - nread;
}

ssize_t 
writen(int fd, void *buf, size_t len){
  size_t nwrite, nleft;
  char *ptr;
  
  ptr = buf;
  nleft = len;
  while(nleft > 0){
    if((nwrite = write(fd, ptr, nleft)) < 0){
      if(errno == EINTR)
	nwrite = 0;
      else 
	return -1;
    }else if(nwrite == 0){
      break;
    }
    nleft -= nwrite;
    ptr += nwrite;
  }
  return len;
}
