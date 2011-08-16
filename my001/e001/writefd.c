#include "unp.h"

ssize_t
write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
  printf("enter write fd\n");
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
  cmptr = CMSG_FIRSTHDR(&msg);
  cmptr->cmsg_len = CMSG_LEN(sizeof(int));
  cmptr->cmsg_level = SOL_SOCKET;
  cmptr->cmsg_type = SCM_RIGHTS;
  *((int *)CMSG_DATA(cmptr)) = sendfd;
  
  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  iov[0].iov_base = ptr;
  iov[0].iov_len = nbytes;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  printf("sendmsg \n");
  return(sendmsg(fd, &msg, 0));
}
