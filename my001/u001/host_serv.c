#include "unp.h"

struct addrinfo *
hot_serv(const char *host, const char *serv, int family, int socktype)
{
  int n;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_CANONNAME;
  hints.ai_family = family;
  hints.ai_socktype = socktype;
  
  if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
    return (NULL);
  
  return (res);
}
/*void
main(void){}
*/
