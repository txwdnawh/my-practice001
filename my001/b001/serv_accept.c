#include <sys/types.h>        
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define QLEN 10

//void main(void){}
serv_accept(int listenfd, uid_t *uidptr)
{
  int err, clifd, len;
  struct sockaddr_un un;
  struct stat statbuf;
  
  len = sizeof(un);
  
  if((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0){
    return (-1);
  }
  
  len -= offsetof(struct sockaddr_un, sun_path);
  un.sun_path[10] = 0;
  
  if(stat(un.sun_path, &statbuf) < 0){
    err = errno;
    close(clifd);
    errno = err;
    return (-2);
  }

  if(S_ISSOCK(statbuf.st_mode) == 0){
    err = errno;
    close(clifd);
    errno = err;
    return (-3);
  }
  
  if((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
     (statbuf.st_mode & S_IRWXU) != S_IRWXU){
    err = errno;
    close(clifd);
    errno = err;
    return (-4);
  }

  if(uidptr != NULL)
    *uidptr = statbuf.st_uid;
  unlink(un.sun_path);
  return(clifd);
}
