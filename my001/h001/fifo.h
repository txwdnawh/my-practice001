#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SERVER_FIFO_NAME "/tmp/server_fifo"  
#define CLIENT_FIFO_NAME "/tmp/client_%d_fifo"  
#define BUFFER_SIZE PIPE_BUF  
#define MESSAGE_SIZE 20  
#define NAME_SIZE 256
#define err_quit(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)
struct message  
{  
  pid_t client_pid;  
  char data[MESSAGE_SIZE + 1];  
};

