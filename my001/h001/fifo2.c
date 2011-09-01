#include "fifo.h"
//#define 
int
main(int argc, char *argv[])
{
  int status;
  int serverfd, clientfd;
  pid_t cli_pid;
  pid_t pid;
  char *p;
  struct message msg;
  char client_fifo_name[NAME_MAX];             
  sleep(1);
  
 again:
  if((pid = fork()) < 0){
    err_quit("fork");
  }else if(pid == 0){
    
    printf("\n\nchild %d\n", getpid());
    snprintf(client_fifo_name, sizeof(client_fifo_name), CLIENT_FIFO_NAME , getpid());
    puts(client_fifo_name);
    if(mkfifo(client_fifo_name, 0777) < 0){
      err_quit("mkfifo");
    }
  
#ifdef DEBUG
    printf(" mkfifo ok\n");
#endif
  
    memset(&msg, 0, sizeof(msg));
    msg.client_pid = getpid();
    snprintf(msg.data, sizeof(msg.data), "hello world");
  
  
  
    if((serverfd = open(SERVER_FIFO_NAME, O_WRONLY)) < 0){
      err_quit("open server error");
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
  
  
    if((clientfd = open(client_fifo_name, O_RDONLY)) < 0){
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
  }else{
    if(waitpid(pid, &status, 0) < 0){
      err_quit("waitpid");
    }
    
    if(WIFEXITED(status)){
      printf("WEXITSTATUS = %d\n", WEXITSTATUS(status));
    }else if(WIFSIGNALED(status)){
      printf("WTERMSIG = %d\n", WTERMSIG(status));
    }else if(WCOREDUMP(status)){
      printf("coredump\n");
    }else{
      printf("i don't konw");
    }
    sleep(5);
    goto again;
    
  }
  
  
  
  
  unlink(client_fifo_name);
  close(serverfd);
  close(clientfd);
  exit(EXIT_SUCCESS);
}

