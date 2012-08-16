/*
 * main.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: ricardo
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "SimpleWebSocketServer.hpp"

using namespace Server;

void initialize();
void loadContent();
void unloadContent();
void update();
void draw();


int main(void)
{
  pthread_t thread_wsServer;
  int t1;

  SimpleWebSocketServer wsServer;
  

  t1 = pthread_create(&thread_wsServer, NULL, SimpleWebSocketServer::runServer(0, NULL), "Websocket Server");
  return EXIT_SUCCESS;
}


void initialize()
{
}

void loadContent()
{
}

void unloadContent()
{
}

void update()
{
}

void draw()
{
}

