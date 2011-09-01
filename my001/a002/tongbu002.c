#include "unp.h"
#include <fcntl.h>
#include <sys/mman.h>

#define NLOOPS 1000
#define SIZE sizeof(long)
static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;
static struct sigaction act;


static int pfd1[2], pfd2[2];
static void
sig_usr(int signo){
  sigflag = 1;
}
void
TELL_WAIT(void){
  if(pipe(pfd1) < 0 || pipe(pfd2) < 0){
    err_quit("pipe");
  }
}

void TELL_PARENT(pid_t pid){
  if(write(pfd2[1], "c", 1) != 1){
    err_quit("write parent");
  }
}

void TELL_CHILD(pid_t pid){
  if(write(pfd1[1], "p", 1) != 1){
    err_quit("write child");
  }
}

void WAIT_PARENT(void){
  char c;
  if(read(pfd1[0], &c, 1) != 1){
    err_quit("read parent");
  }
  
  if(c != 'p'){
    err_quit("WAIT_PARENT: incorrect data");
  }
}

void WAIT_CHILD(void){
  char c;
  if(read(pfd2[0], &c, 1) != 1){
    err_quit("read child");
  }
  
  if(c != 'c'){
    err_quit("WAIT_CHILD: incorrect data");
  }
}
static int
update(long *ptr){
  return ((*ptr)++);
}


static void
print_char(char *str){
  char *ptr;
  int  c;
  
  //  setbuf(stdout, NULL);
  for(ptr = str; (c = *ptr) != 0; ptr++){
    putc(c, stdout);
  }
}
int
main(int argc, char *argv[])
{
  int fd, i, counter;
  pid_t pid;
  void *area;
  // setbuf(stdout, NULL);
  //  if((fd = open("/dev/zero", O_RDWR)) < 0){
  //   err_quit("open");
  //  }
  if((area = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
    err_quit("mmap");
  }
  //  close(fd);
  
  TELL_WAIT();
  if((pid = fork()) < 0){
    err_quit("fork");
  }else if(pid == 0){
    
    
    
    
    for(i = 1; i < NLOOPS + 1; i += 2){
      WAIT_PARENT();
      
      if((counter = update((long *)area)) != i){
	fprintf(stderr, "%d got %d\n", i, counter);
	err_quit("update child");
      }
      fprintf(stderr, "child %d got %d\n", i, counter);
      printf("a");
      TELL_PARENT(getppid());
    }
    
    
    
  }else{
      
    for(i = 0; i < NLOOPS + 1; i += 2){
      
      if((counter = update((long *)area)) != i){
	fprintf(stderr, "%d got %d\n", i, counter);
	err_quit("update parent");
      }
      fprintf(stderr, "parent %d got %d\n", i, counter);
      printf("b");
      
      if(i >= 1000){
	exit(0);
      }
      TELL_CHILD(pid);
      WAIT_CHILD();
      
    }
    
    



  
  }
  
  exit(0);
}

