/* include write_fd */
#include	"unp.h"
#define _GNU_SOURCE
#include "sys/socket.h"

ssize_t
write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
  struct msghdr	msg;
	struct iovec	iov[1];

	

	union {
	  struct cmsghdr	cm;
	  char				control[CMSG_SPACE(sizeof(struct ucred)) + CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr	*cmptr;
	struct ucred cred, mycred;
	
	cred.uid = geteuid();
	cred.gid = getegid();
	cred.pid = getpid();
	
	printf("writefd: pid = %d\n\n", cred.pid);
	
	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	
	cmptr = CMSG_FIRSTHDR(&msg);
	
	cmptr->cmsg_len = CMSG_LEN(sizeof(struct ucred));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_CREDENTIALS;
	*((struct ucred *) CMSG_DATA(cmptr)) = cred;
	mycred = *((struct ucred *)CMSG_DATA(cmptr));
	printf("mycred.pid = %d\n", mycred.pid);
	
	cmptr = CMSG_NXTHDR(&msg, cmptr);
	
	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	*((int *) CMSG_DATA(cmptr)) = sendfd;
	


	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	
	return(sendmsg(fd, &msg, 0));
}
/* end write_fd */

