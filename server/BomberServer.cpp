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

  width = 640;
  height = 480;

  
  
  objects.push_back(gameObj(pair<int,int> (1,1),square));
  objects.push_back(gameObj(pair<int,int> (1,3),square));
  objects.push_back(gameObj(pair<int,int> (1,5),square));
  objects.push_back(gameObj(pair<int,int> (1,7),square));
  objects.push_back(gameObj(pair<int,int> (1,9),square));

  objects.push_back(gameObj(pair<int,int> (3,1),square));
  objects.push_back(gameObj(pair<int,int> (3,3),square));
  objects.push_back(gameObj(pair<int,int> (3,5),square));
  objects.push_back(gameObj(pair<int,int> (3,7),square));
  objects.push_back(gameObj(pair<int,int> (3,9),square));

  objects.push_back(gameObj(pair<int,int> (5,1),square));
  objects.push_back(gameObj(pair<int,int> (5,3),square));
  objects.push_back(gameObj(pair<int,int> (5,5),square));
  objects.push_back(gameObj(pair<int,int> (5,7),square));
  objects.push_back(gameObj(pair<int,int> (5,9),square));

  objects.push_back(gameObj(pair<int,int> (7,1),square));
  objects.push_back(gameObj(pair<int,int> (7,3),square));
  objects.push_back(gameObj(pair<int,int> (7,5),square));
  objects.push_back(gameObj(pair<int,int> (7,7),square));
  objects.push_back(gameObj(pair<int,int> (7,9),square));

  objects.push_back(gameObj(pair<int,int> (9,1),square));
  objects.push_back(gameObj(pair<int,int> (9,3),square));
  objects.push_back(gameObj(pair<int,int> (9,5),square));
  objects.push_back(gameObj(pair<int,int> (9,7),square));
  objects.push_back(gameObj(pair<int,int> (9,9),square));

  players.push_back(player(gameObj (pair<int,int> (0,0),bomber),0));

  //concurrency problems?
  for(vector<gameObj>::iterator it = objects.begin(); it != objects.end(); it++)
    {
      if(((gameObj)*it).second == square)
	{
	  stringstream str;
	  str << "sq_" << ((gameObj)*it).first.first << "," << ((gameObj)*it).first.second;
	  msgs.push_back(str.str());
	}
    }

  stringstream str;
  str << "bomber_0,0";
}

BomberServer::~BomberServer() {
}

void BomberServer::step() {
  this_thread::sleep_for(std::chrono::milliseconds(1000/120));
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

  if(msg.compare("leftKey")==0)
    {
      players[0].first.first.first += -5;
      msgs.push_back("player 0 leftKey");
    }
  else if(msg.compare("rightKey")==0)
    {
      players[0].first.first.first += 5;
      msgs.push_back("player 0 rightKey");
    }
  else if(msg.compare("upKey")==0)
    {
      players[0].first.first.second += -5;
      msgs.push_back("player 0 upKey");
    }
  else if(msg.compare("downKey")==0)
    {
      players[0].first.first.second += 5;
      msgs.push_back("player 0 downKey");
    }
}
