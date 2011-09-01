#include "unp.h"

int
main(int argc, char *argv[])
{
  unsigned char addr[sizeof(struct sockaddr_in)];
  char str[INET_ADDRSTRLEN];
  int a, b, c, d;
  char hexString[9]; 
  inet_pton(AF_INET, argv[1], addr);
  //printf("inet_pton(AF_INET, argv[1], addr) = %x\n", addr);
  printf("%s\n",inet_ntop(AF_INET, addr, str, INET_ADDRSTRLEN));
  
  sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
  printf("%d%d%d%d", a, b, c,d);
  sprintf( hexString, "%02x %02x %02x %02x", a, b, c, d ); ;
  
  printf("\n%s\n", hexString);
  exit(EXIT_SUCCESS);
  
}


