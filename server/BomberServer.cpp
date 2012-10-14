/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include <iostream>
#include <sstream>
#include "BomberServer.hpp"

using namespace std;

BomberServer::BomberServer() {
  //criar a fase

  objects.push_back(new gameObj((1,1),square));
  objects.push_back(new gameObj((1,3),square));
  objects.push_back(new gameObj((1,5),square));
  objects.push_back(new gameObj((1,7),square));
  objects.push_back(new gameObj((1,9),square));

  objects.push_back(new gameObj((3,1),square));
  objects.push_back(new gameObj((3,3),square));
  objects.push_back(new gameObj((3,5),square));
  objects.push_back(new gameObj((3,7),square));
  objects.push_back(new gameObj((3,9),square));

  objects.push_back(new gameObj((5,1),square));
  objects.push_back(new gameObj((5,3),square));
  objects.push_back(new gameObj((5,5),square));
  objects.push_back(new gameObj((5,7),square));
  objects.push_back(new gameObj((5,9),square));

  objects.push_back(new gameObj((7,1),square));
  objects.push_back(new gameObj((7,3),square));
  objects.push_back(new gameObj((7,5),square));
  objects.push_back(new gameObj((7,7),square));
  objects.push_back(new gameObj((7,9),square));

  objects.push_back(new gameObj((9,1),square));
  objects.push_back(new gameObj((9,3),square));
  objects.push_back(new gameObj((9,5),square));
  objects.push_back(new gameObj((9,7),square));
  objects.push_back(new gameObj((9,9),square));

  players.push_back(new player(((0,0),bomber),0));

  //concurrency problems?
  for(vector<gameObjs>::iterator it = objects.begin(); it != objects.end(); it++)
    {
      if(*it.second == square)
	{
	  stringstream str;
	  str << "sq_" << *it.first.first << "_" << *it.first.second;
	  msgs.push_back(str.str());
	}
    }

  stringstream str;
  str << "bomber_" << *it.first.first << "_" << *it.first.second;
}

BomberServer::~BomberServer() {
}

void BomberServer::step() {
  this_thread::sleep_for(1000/120);
  for(vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++)
    broadcast(*it);
}

void BomberServer::onClientConnect(ClientConnection * client) {
    stringstream smsg;
    smsg << "client " << client->id << " connected" << endl;
    broadcast(smsg.str());
}

void BomberServer::onClientDisconnect(ClientConnection * client) {

}

void BomberServer::onNewMessage(std::string msg) {

  if(msg.compare(msg, "leftKey")==0)
    {
      players[0].first.first -= 5;
      msgs.push_back("player 0 leftKey");
    }
  else if(msg.compare(msg, "rightKey")==0)
    {
      players[0].first.first += 5;
      msgs.push_back("player 0 rightKey");
    }
  else if(msg.compare(msg, "upKey")==0)
    {
      players[0].first.second -= 5;
      msgs.push_back("player 0 upKey");
    }
  else if(msg.compare(msg, "downKey")==0)
    {
      players[0].first.second += 5;
      msgs.push_back("player 0 downKey");
    }
}
