#include "fifo.h"
//#define 

static char client_fifo_name[NAME_SIZE];
static void
sig_int(int signo)
{
  if(signo == SIGINT){
    printf("catch sigint");
    if(unlink(SERVER_FIFO_NAME) < 0){
      perror("sigint ");
      _exit(1);
    }
  }
}

int
main(int argc, char *argv[])
{
  int serverfd, clientfd;
  pid_t cli_pid;
  char *p;
  struct message msg;
  struct sigaction act, save_int_act, save_quit_act;
  
  act.sa_handler = sig_int;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGQUIT);
  act.sa_flags = SA_RESTART;
  
  if(sigaction(SIGINT, &act, &save_int_act) < 0){
    err_quit("sigaction");
  }
  
  act.sa_handler = sig_int;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGINT);
  act.sa_flags = 0;
  
  if(sigaction(SIGQUIT, &act, &save_quit_act) < 0){
    err_quit("sigaction");
  }
  
  
  if(mkfifo(SERVER_FIFO_NAME, 0777) < 0){
    err_quit("mkfifo");
  }
  
#ifdef DEBUG
  printf("mkfifo ok\n");
#endif
  
 
  if((serverfd = open(SERVER_FIFO_NAME, O_RDONLY)) < 0){
    
    err_quit("open serverfifo error");
  }
  
#ifdef DEBUG
  printf("open serverfifo ok\n");
#endif
  
  for(;;){
  if(read(serverfd, &msg, sizeof(msg)) > 0){
    
    
#ifdef DEBUG
    printf("read serverfifo ok\n");
#endif
    cli_pid = msg.client_pid;
    p = msg.data;
    printf("recv %s from client %ld\n", p, cli_pid);
    
    while(*p){
      *p = toupper(*p);
      ++p;
    }
    
    snprintf(client_fifo_name, sizeof(client_fifo_name), CLIENT_FIFO_NAME, msg.client_pid);
    if((clientfd = open(client_fifo_name, O_WRONLY)) < 0){
      err_quit("open client error");
    }
    
#ifdef DEBUG
    printf("open clientfifo ok\n");
#endif
    if(write(clientfd, &msg, sizeof(msg)) < 0){
      err_quit("write client error");
    }
    
#ifdef DEBUG
    printf("write clientfifo ok\n");
#endif
  }
  
  }
  
  if(sigaction(SIGINT, &save_int_act, NULL) < 0){
    err_quit("sigaction");
  }
  if(sigaction(SIGQUIT, &save_quit_act, NULL) < 0){
    err_quit("sigaction");
  }
  
  unlink(SERVER_FIFO_NAME);
  close(serverfd);
  close(clientfd);
  exit(EXIT_SUCCESS);
}

