/* include read_fd */
#include	"unp.h"

ssize_t
read_fd(int fd, void *ptr, size_t nbytes, int *recvfd, struct ucred *cred)
{
  struct msghdr	msg;
  struct iovec	iov[1];
  ssize_t			n;
  int				newfd;
  

    const int on = 1;
  
  if(setsockopt(fd, SOL_SOCKET, SO_PASSCRED, &on, sizeof(int)) < 0){
   perror("setsockopt error");
   exit(1);
  }
  
  
  union {
    struct cmsghdr	cm;
    char control[ CMSG_SPACE(sizeof(struct ucred)) + CMSG_SPACE(sizeof(int))];
  } control_un;
    struct cmsghdr	*cmptr;
    
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    
    
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    
    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    if ( (n = recvmsg(fd, &msg, 0)) <= 0)
      return(n);
    
    
    cmptr = CMSG_FIRSTHDR(&msg);
    
    printf("1\n");
    if(cmptr->cmsg_type == SCM_CREDENTIALS){
      printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
      *cred = *((struct ucred*)CMSG_DATA(cmptr));
    }
    
    cmptr = CMSG_NXTHDR(&msg, cmptr);
    
    *recvfd = *((int *)CMSG_DATA(cmptr));
    
    
    
    return(n);
}
/* end read_fd */
