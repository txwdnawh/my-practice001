#include "unp.h"

int
main(int argc, char *argv[])
{
  mqd_t mqdes;
  char buf[] = "hello world";
  umask(0);
  if((mqdes = mq_open(argv[1], O_RDWR|O_CREAT, 0660, NULL)) < 0){
    err_quit("mq_open");
  }
  
  printf("sleep\n");
  sleep(10);
  
  if(mq_send(mqdes, buf, sizeof(buf), 10) < 0){
    err_quit("mq_send");
  }
  
  printf("mq_send ok\n");
  if(mq_close(mqdes) < 0){
    err_quit("mq_close");
  }
  pause();
  exit(EXIT_SUCCESS);
}
