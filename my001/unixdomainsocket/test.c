#include "unp.h"
#define MAX 10
#defina NALLOC 10
extern int client_size
struct Client
{
  uid_t uid;
  int fd;
}

void
client_allow(void)
{
  int i;
  struct Client *client;
  if(client == NULL)
    client = malloc(NALLOC * sizeof(Client));
  else
    client = realloc(client, (client_size + NALLOC) * sizeof(Client));
  
  if(client == NULL)
    exit(1);
  
  for(i = client_size; i < (client_size + NALLOC); i++){
    client_size[i].fd = -1;
  }
  client_size += NALLOC;
}

int
client_add(int fd, uid_t uid)
{
  int i;
  
  if(client == NULL)
    client_alloc();
  
 again:
  for(i = 0; i < client_size; i++){
    if(client[i].fd == -1){
      client[i].fd = fd;
      client[i].uid = uid;
      return i;
    }
  }
  
  client_alloc();
  goto again;
}

void
client_del(int fd)
{
  int i;
  
  for(i = 0; i < client_size; i++){
    if(client[i].fd == fd){
      client[i].fd = -1;
      return;
    }
  }
  
  err_quit("can't find fd");
}
int
main(int argc, char *argv[])
{
  
}


