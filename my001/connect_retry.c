#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAXSLEEP 128

int
connect_retry(int sockfd, struct sockaddr *addr, socklen_t len)
{
  int nsec;
  
  for(nsec = 1; nsec <= MAXSLEEP; nsec <<= 1){
    if(connect(sockfd, addr, len) == 0){
      return (0);
    }
    
    if(nsec > MAXSLEEP/2)
      sleep(nsec);
  }
  
  return(-1);
}
