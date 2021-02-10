#define TCP_MINI_SCOUT
#include "tcp_mini.h"

#include "buffer_message.h"

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

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

enum
{
	EAttemptConnectResult_Connected = 1,
	EAttemptConnectResult_FailedToConnect
};
int attemptConnectResult = 0; //< 0 means pending

void* attempt_connect(void* a)
{
	struct tm_match_blob_t* b = (struct tm_match_blob_t*) a;

	if(tm_connect(*b) != 1)
	{
		if(errno == ECONNREFUSED)
		{
			// .. (we were refused, there might not be a server with the hostname/ip and port)
		}
		attemptConnectResult = EAttemptConnectResult_FailedToConnect;
		return NULL;
	}

	attemptConnectResult = EAttemptConnectResult_Connected;
	return NULL;
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

  pthread_t d;
  pthread_create(&d, NULL, &attempt_connect, &a);

  while(1)
  {
	  switch(attemptConnectResult)
	  {
	  case EAttemptConnectResult_FailedToConnect:
		  printf("failed to connect\n");
		  return 1;
	  case 0: //< pending
		  sleep(1);
		  continue;
	  }

	  if(attemptConnectResult == EAttemptConnectResult_Connected)
	  {
		  break;
	  }
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
