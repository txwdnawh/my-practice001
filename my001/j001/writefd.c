#include "unp.h"
int
write_fd(int fd, void *buf, size_t nbytes, int sendfd)
{
  struct msghdr msg;
  struct iovec iov[1];
  
  union{
    struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int))];
  }control_un;
  struct cmsghdr *cmptr;
  
  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  iov[0].iov_base = buf;
  iov[0].iov_len = nbytes;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control_un.control;
  msg.msg_controllen = sizeof(control_un);
  
  cmptr = CMSG_FIRSTHDR(&msg);
  cmptr->cmsg_len = CMSG_LEN(sizeof(int));
  cmptr->cmsg_level = SOL_SOCKET;
  cmptr->cmsg_type = SCM_RIGHTS;
  *((int *)CMSG_DATA(cmptr)) = sendfd;
  return (sendmsg(fd, &msg, 0));
}
