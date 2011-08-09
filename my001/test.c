#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stddef.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>

/*int main(int argc, char *argv){
FILE *p=fopen("abcd","w+");
fputs("abcde", p );
fclose(p);

}
*/

#include <stdio.h>

char a,b,c,d,e,f;
int i=0;

main()

{

  FILE *p=fopen("abcd","w+");
  for(a=1;a<7;a++)
for(b=1;b<7;b++)
for(c=1;c<7;c++)
for(d=1;d<7;d++)
for(e=1;e<7;e++)
for(f=1;f<7;f++)
{if((a!=b)&&(a!=c)&&(a!=d)&&(a!=e)&&(a!=f))
if((b!=c)&&(b!=d)&&(b!=e)&&(b!=f))
if((c!=d)&&(c!=e)&&(c!=f))
if((d!=e)&&(d!=f))
  if(e!=f){i++;fprintf(p, "PASSWORD %d:%d%d%d%d%d%d\n",i,a,b,c,d,e,f);}
}
  
fclose(p);
}
