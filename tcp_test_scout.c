#define TCP_MINI_SCOUT
#include "tcp_mini.h"

#include "buffer_message.h"

#include <string.h>

#include <errno.h>

int main()
{
  struct tm_match_blob_t a;
  //if(!tm_search_for_match("127.0.0.1:1025", &a))
  //{
  //	return -1;
  //}
  //strcpy(a.ip, "127.0.0.1");
  strcpy(a.hostname, "localhost");
  a.port = 44444;

  printf("started\n");

  if(tm_connect(a) != 1)
  {
	if(errno == ECONNREFUSED)
	{
		// .. (we were refused, there might not be a server with the hostname/ip and port)
	}
    return -1;
  }

  printf("connected\n");

  struct unformattedbuffer_t b = unformattedbuffer_default;
  char* c = "hello";
  b.size = sizeof c;
  TM_SEND_BLOCK(&b, c);

  tm_disconnect();

  printf("disconnected\n");
}
