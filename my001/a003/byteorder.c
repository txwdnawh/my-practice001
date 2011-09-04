#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int
main(int argc, char *argv[])
{
  uint32_t i;
  unsigned char *cp;
  
  i = 0x04030201;
  cp = (unsigned char *)&i;
  if(*cp ==1){
    printf("little endian\n");
  }else if(*cp == 4){
    printf("big endian\n");
  }else{
    printf("unkonw\n");
  }
  exit(EXIT_SUCCESS);
}


/*
int
main(int argc, char **argv)
{
  union {
    short  s;
    char   c[sizeof(short)];
  } un;

  un.s = 0x0102;
  if (sizeof(short) == 2) {
    if (un.c[0] == 1 && un.c[1] == 2)
      printf("big-endian\n");
    else if (un.c[0] == 2 && un.c[1] == 1)
      printf("little-endian\n");
    else
      printf("unknown\n");
  } else
    printf("sizeof(short) = %d\n", sizeof(short));

  exit(0);
}
*/
