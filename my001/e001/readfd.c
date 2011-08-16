#include "unp.h"

ssize_t
read_fd(int fd, void *ptr, size_t nbytes, int *recvfd)
{
  printf("enter read fd\n");
  struct msghdr msg;
  struct iovec iov[1];
  size_t n;
  
  int newfd;
  
  union{
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  }control_un;
  struct cmsghdr *cmptr;
  
  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof(control_un.control);
  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  iov[0].iov_base = ptr;
  iov[0].iov_len = nbytes;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  if((n = recvmsg(fd, &msg, 0)) <= 0)
    return (n);
  
  if((cmptr = CMSG_FIRSTHDR(&msg)) != NULL && cmptr->cmsg_len == CMSG_LEN(sizeof(int))){
    if(cmptr->cmsg_level != SOL_SOCKET){
      perror("cmsg_level error");
      exit(1);
    }
    if(cmptr->cmsg_type != SCM_RIGHTS){
      perror("cmsg_type error");
      exit(1);
    }
    printf("recvfd = xxx\n");
    *recvfd = *((int *)CMSG_DATA(cmptr));
    printf("aaaaaaaaaaaaaaaaaaaaaaa%d\n", *recvfd);
  }else{
    *recvfd = 01;
  }
  return (n);
}
