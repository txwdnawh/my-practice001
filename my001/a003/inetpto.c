#include "unp.h"


int
main(int argc, char *argv[])
{
  union{
    unsigned char addr[sizeof(struct sockaddr_in)];
    uint8_t arr[4];
  }x;
  char str[INET_ADDRSTRLEN];
  int i, a, b, c, d;
  char hexString[9]; 
  inet_pton(AF_INET, argv[1], x.addr);
  //printf("inet_pton(AF_INET, argv[1], addr) = %x\n", addr);
  printf("%s\n",inet_ntop(AF_INET, x.addr, str, INET_ADDRSTRLEN));
  
  sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
  printf("%d%d%d%d", a, b, c,d);
  sprintf( hexString, "%02x %02x %02x %02x", a, b, c, d ); ;
  printf("\n%s\n", hexString);
  
  for(i = 0; i < 4; i ++){
    printf("0x%d\n", x.arr[i]);
  }
  exit(EXIT_SUCCESS);
  
}


