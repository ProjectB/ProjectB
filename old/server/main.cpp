/*
 * main.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: ricardo
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#if defined(__cplusplus)
extern "C"
#endif


#include "SimpleWebSocketServer.hpp"

using namespace Server;

void *callWSServer(void*);
void initialize();
void loadContent();
void unloadContent();
void update();
void draw();


int main(void)
{
  pthread_t thread_wsServer;
  void* wtf;
  int t1;

  t1 = pthread_create(&thread_wsServer, NULL, &callWSServer, NULL);

  pthread_join(t1, &wtf);
  return t1;
}

void *callWSServer(void* x)
{
  char *msg;
  msg = (char*)x;
  while(1)
    cout << msg << endl;
  return x;
}


void initialize()
{
  return;
}

void loadContent()
{
  return;
}

void unloadContent()
{
  return;
}

void update()
{
  return;
}

void draw()
{
  return;
}

