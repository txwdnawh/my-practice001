#include "fifo.h"
#define 
int
main(int argc, char *argv[])
{
  int serverfd, clientfd;
  pid_t cli_pid;
  char *p;
  struct message msg;
  
  
  if(mkfifo(CLIENT_FIFO_NAME, 0777) < 0){
    err_quit("mkfifo");
  }
  
#ifdef DEBUG
  printf(" mkfifo ok\n");
#endif
  
  memset(&msg, 0, sizeof(msg));
  msg.client_pid = getpid();
  snprintf(msg.data, sizeof(msg.data), "hello world");
  
  
  
  if((serverfd = open(SERVER_FIFO_NAME, O_WRONLY)) < 0){
    err_quit("open client error");
  }  
  
#ifdef DEBUG
  printf("open serverfifo ok\n");
#endif
  
  if(write(serverfd, &msg, sizeof(msg)) < 0){
    err_quit("write client error");
  }
  
#ifdef DEBUG
  printf("write clientfifo ok\n");
#endif
  
  if((clientfd = open(CLIENT_FIFO_NAME, O_RDONLY)) < 0){
    err_quit("open clientfifo error");
  }
  
#ifdef DEBUG
  printf("open clientfifo ok\n");
#endif
  if(read(clientfd, &msg, sizeof(msg)) > 0){
    
#ifdef DEBUG
    printf("read clientfifo ok\n");
#endif
    p = msg.data;
    printf("recv %s from server \n", msg.data);
  }
  
  unlink(CLIENT_FIFO_NAME);
  close(serverfd);
  close(clientfd);
  exit(EXIT_SUCCESS);
}

