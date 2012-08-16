/*
 * main.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: ricardo
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

<<<<<<< HEAD
#include "SimpleWebSocketServer.cpp"
=======
#include "SimpleWebSocketServer.hpp"

using namespace Server;
>>>>>>> kodo

void initialize();
void loadContent();
void unloadContent();
void update();
void draw();


int main(void)
{
<<<<<<< HEAD
  pthread_t wsServer;
  int t1;

  //  t1 = pthread_create(&wsServer, NULL, 
=======
  pthread_t thread_wsServer;
  int t1;

  SimpleWebSocketServer wsServer;
  

  t1 = pthread_create(&thread_wsServer, NULL, SimpleWebSocketServer::runServer(0, NULL), "Websocket Server");
>>>>>>> kodo
  return EXIT_SUCCESS;
}


void initialize()
{
<<<<<<< HEAD
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

=======
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

>>>>>>> kodo
void draw()
{
}

