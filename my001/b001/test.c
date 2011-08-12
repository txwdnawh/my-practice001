#include <string.h>
#include <stdio.h>
#define WHITE " \t\n"
int
main(int argc, char *argv[]){
  
  char str[] = "now#is the time for all#good men to come to the # aid of their country";
  char delims[] = "#";
  char *result = NULL;
  result = strtok( str, delims );
  while( result != NULL ) {
    printf("---%s---\n", str);
    printf( "result is \"%s\"\n", result );
    result = strtok( NULL, delims );
    
  }
  return 0;
}
