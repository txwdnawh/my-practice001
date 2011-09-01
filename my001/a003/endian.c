#include "unp.h"

int
main(int argc, char *argv[])
{
  union{
    uint32_t u32;
    uint8_t arr[4];
  }x;
  
  /* 2 wei 16 jinzhi dengyu 8 wei 2 jinzhi */
  x.arr[0] = 0x11; 
  x.arr[1] = 0x22;
  x.arr[2] = 0x33;
  x.arr[3] = 0x44;
  
  int i;
  
  for(i = 0; i < 4; i++){
    printf("0x%x\n",x.arr[i]);
  }
  
  printf("x.u32 = 0x%X\n", x.u32);
  printf("htole32(x.u32) = 0x%x\n", htole32(x.u32));
  printf("htobe32(x.u32) = 0x%x\n", htobe32(x.u32));
  exit(EXIT_SUCCESS);
}
