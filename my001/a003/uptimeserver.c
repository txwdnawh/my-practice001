#include "unp.h"
#include <syslog.h>
#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int ,struct sockaddr *addr, socklen_t alen);
/*
void
serve(int sockfd)
{
  int clientfd;
  FILE *fp;
  char buf[BUFLEN];
  
  for(;;){
    clientfd = accept(sockfd, NULL, NULL);
    if(clientfd < 0){
      syslog(LOG_ERR|LOG_DAEMON, "ruptimed: accept error:%s", strerror(errno));
      exit(1);
    }
    if((fp = popen("/usr/bin/uptime", "r")) == NULL){
      snprintf(buf, strlen(buf), "error: %s\n", strerror(errno));
      if(send(clientfd, buf, strlen(buf), 0) < 0){
	syslog(LOG_ERR|LOG_DAEMON, "ruptime send error: %s", strerror(errno));
	exit(1);
      }
    }else{
      while(fgets(buf, BUFLEN, fp) != NULL){
	if(send(clientfd, buf, strlen(buf), 0) < 0){
	  syslog(LOG_ERR|LOG_DAEMON, "ruptime send error: %s", strerror(errno));
	  exit(1);
	}
      }
      pclose(fp);
    }
    close(clientfd);
  }
}
*/


 /*
void
serve(int sockfd)
{
  int clientfd, status;
  pid_t pid;
  
  for(;;){
    clientfd = accept(sockfd, NULL, NULL);
    if(clientfd < 0){
      syslog(LOG_ERR|LOG_DAEMON, "ruptimed: accept error:%s", strerror(errno));
      exit(1);
    }
    if((pid = fork()) < 0){
      syslog(LOG_ERR | LOG_DAEMON, "ruptime fork error: %s", strerror(errno));
      exit(1);
    }else if(pid == 0){
      if(clientfd != STDIN_FILENO){
	if(dup2(clientfd, STDIN_FILENO) != STDIN_FILENO){
	  syslog(LOG_ERR|LOG_DAEMON, "ruptimed: error");
	  exit(1);
	}
      }
      if(clientfd != STDOUT_FILENO){
	if(dup2(clientfd, STDOUT_FILENO) != STDOUT_FILENO){
	  syslog(LOG_ERR|LOG_DAEMON, "ruptimed: error");
	  exit(1);
	}
      }
      close(clientfd);
      execl("/usr/bin/uptime", "uptime", (char *)0);
      syslog(LOG_ERR|LOG_DAEMON, "ruptime: unexpected return from exec: %s", strerror(errno));
    }else{
      close(clientfd);
      if(waitpid(pid, &status, 0) < 0){
	syslog(LOG_ERR|LOG_DAEMON, "waitpid error");
	exit(1);
      }
    }
  }
}
 */


void
sig_child(int signo)
{
  while(waitpid((pid_t)-1, NULL, WNOHANG) > 0){
    ;
  }
}
void
serve(int sockfd)
{
  int clientfd, status;
  pid_t pid;
  
  for(;;){
    clientfd = accept(sockfd, NULL, NULL);
    if(clientfd < 0){
      syslog(LOG_ERR|LOG_DAEMON, "ruptimed: accept error:%s", strerror(errno));
      exit(1);
    }
    if((pid = fork()) < 0){
      syslog(LOG_ERR | LOG_DAEMON, "ruptime fork error: %s", strerror(errno));
      exit(1);
    }else if(pid == 0){
      if(clientfd != STDIN_FILENO){
	if(dup2(clientfd, STDIN_FILENO) != STDIN_FILENO){
	  syslog(LOG_ERR|LOG_DAEMON, "ruptimed: error");
	  exit(1);
	}
      }
      if(clientfd != STDOUT_FILENO){
	if(dup2(clientfd, STDOUT_FILENO) != STDOUT_FILENO){
	  syslog(LOG_ERR|LOG_DAEMON, "ruptimed: error");
	  exit(1);
	}
      }
      close(clientfd);
      execl("/usr/bin/uptime", "uptime", (char *)0);
      syslog(LOG_ERR|LOG_DAEMON, "ruptime: unexpected return from exec: %s", strerror(errno));
    }else{
      close(clientfd);
    }
  }
}
int
main(int argc, char *argv[])
{
  struct addrinfo *ailist, *aip;
  struct addrinfo hint;
  int sockfd, err, n;
  char *host;
  struct sigaction act;
  
  act.sa_handler = sig_child;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;    //accept Interrupted by SIGCHLD
  if(sigaction(SIGCHLD, &act, NULL) < 0){
    err_quit("sigaction");
  }
  
  
#ifdef _SC_HOST_NAME_MAX
  n = sysconf(_SC_HOST_NAME_MAX);
  if(n < 0)
#endif
    n = HOST_NAME_MAX;
  
  host = malloc(n);
  if(host == NULL){
    err_quit("malloc");
  }
  
  if(gethostname(host, n) < 0){
    err_quit("gethostname");
  }
  
  
  hint.ai_flags = AI_CANONNAME;
  hint.ai_family = 0;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_addrlen = 0;
  hint.ai_canonname =NULL;
  hint.ai_addr = NULL;
  hint.ai_next = NULL;
  
  if((err = getaddrinfo("127.0.0.1", "9999", &hint, &ailist)) != 0){
    syslog(LOG_ERR|LOG_DAEMON, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
    exit(1);
  }
  
 
  for(aip = ailist; aip != NULL; aip = aip->ai_next){
    if((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen)) < 0){
      printf("bbb\n");
      err_quit("init server");
    }else{
      serve(sockfd);
      exit(0);
    }
  }
  exit(1);
}
