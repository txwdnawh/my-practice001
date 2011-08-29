#include "fifo.h"
#define 
int
main(int argc, char *argv[])
{
  int serverfd, clientfd;
  pid_t cli_pid;
  char *p;
  struct message msg;
  
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
    
    
    if((clientfd = open(CLIENT_FIFO_NAME, O_WRONLY)) < 0){
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
  
  unlink(SERVER_FIFO_NAME);
  close(serverfd);
  close(clientfd);
  exit(EXIT_SUCCESS);
}

