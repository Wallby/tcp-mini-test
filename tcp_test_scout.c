#define TCP_MINI_SCOUT
#include "tcp_mini.h"

#include "buffer_message.h"

#include <string.h>

#include <errno.h>

void my_on_receive(struct tm_message_t* message, int a)
{
}

int bHasHungUp = 0;
void my_on_hung_up()
{
  bHasHungUp = 1;

  printf("match has hung up\n");
}

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

  tm_set_on_receive(my_on_receive);
  TM_SET_ON_HUNG_UP(my_on_hung_up);

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
  b.size = strlen(c);
  TM_SEND(&b, c, b.size);

  while(!bHasHungUp)
  {
	  tm_poll(1);
  }

  tm_disconnect();

  printf("disconnected\n");

  TM_UNSET_ON_HUNG_UP();
  tm_unset_on_receive();
}
