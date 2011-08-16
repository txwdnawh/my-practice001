#include "unp.h"
#include <syslog.h>
#define MAXFD 1024


void
daemon_init(const char *cmd, int facility)
{
  //1
  pid_t pid;
  struct sigaction act;
  struct rlimit rl;
  int i, fd, fd0, fd1, fd2;
  
  if((pid = fork()) < 0){
    perror("fork");
    exit(1);
  }else if(pid != 0){
    exit(1);
  }
  setsid();
  
  
  act.sa_handler = SIG_IGN;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if(sigaction(SIGHUP, &act, NULL) < 0){
    perror("sigaction");
    exit(1);
  }
  if((pid = fork()) < 0){
    perror("fork2");
    exit(1);
  }else if(pid != 0){
    exit(1);
  }
  
  //2
  umask(0);
  
  //3
  chdir("/");
  
  //4
  
  if(getrlimit(RLIMIT_NOFILE, &rl) < 0){
    perror("getrlimit");
    exit(1);
  }

  if(rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = MAXFD;
  
  for(i = 0; i < rl.rlim_max; i++)
    close(i);
  
  openlog(cmd, LOG_PID, facility);
  //5
  
  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);
  
  if(fd0 != 0 || fd1 != 1 || fd2 != 2){
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
    exit(1);
  }
  
}

//void main(void){}
