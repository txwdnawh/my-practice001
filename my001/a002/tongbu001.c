#include "unp.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define NLOOPS 1000
#define SIZE sizeof(long)
static sem_t *semptr = NULL;
void
TELL_WAIT(void){
  
  sem_unlink("mysem");
  if((semptr = sem_open("mysem", O_CREAT|O_EXCL, 0777, 0)) == SEM_FAILED){
    err_quit("sem_open");
  }
}

void TELL_PARENT(pid_t pid){
  if(sem_post(semptr) < 0){
    err_quit("sem_post error");
  }
}

void TELL_CHILD(pid_t pid){
  if(sem_post(semptr) < 0){
    err_quit("sem_post error");
  }
}

void WAIT_PARENT(void){
  if(sem_wait(semptr) < 0){
    err_quit("sem_wait");
  }
}

void WAIT_CHILD(void){
  if(sem_wait(semptr) < 0){
    err_quit("sem_wait");
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
  
  setbuf(stdout, NULL);
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
  
  if((fd = open("/dev/zero", O_RDWR)) < 0){
    err_quit("open");
  }
  if((area = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
    err_quit("mmap");
  }
  close(fd);
  
  TELL_WAIT();
  if((pid = fork()) < 0){
    err_quit("fork");
  }else if(pid == 0){
    
    /*
    
    for(i = 1; i < NLOOPS + 1; i += 1){
      if((counter = update((long *)area)) != i){
	fprintf(stderr, "%d got %d\n", i, counter);
	err_quit("update child");
      }
    }
    
    
    */
    
     WAIT_PARENT();
    print_char("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    // TELL_PARENT(getppid());
      //  sleep(1);
  }else{
    /*
    sleep(1);
    
    for(i = 0; i < NLOOPS + 1; i += 1){
      if((counter = update((long *)area)) != i){
	fprintf(stderr, "%d got %d\n", i, counter);
	err_quit("update parent");
      }
    }



    */
    // WAIT_CHILD();
    print_char("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
     TELL_CHILD(pid);
  }
  
  exit(0);
}

