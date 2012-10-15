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

 
}

BomberServer::~BomberServer() {
}

void BomberServer::step() {
  //server roda a 120fps
  this_thread::sleep_for(std::chrono::milliseconds(1000/120));
  for(vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++)
    broadcast(*it);
}

void BomberServer::onClientConnect(ClientConnection * client) {
    stringstream smsg;
    smsg << "client " << client->id << " connected" << endl;
    broadcast(smsg.str());

    width = 440;
    height = 440;
    xPoint = width/11;
    yPoint = height/11; 
    xMove = xPoint/5;
    yMove = yPoint/5;
    
    
    newObject(xPoint, yPoint, xPoint-2, yPoint-2, square);
    newObject(xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
    newObject(xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
    newObject(xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
    newObject(xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

    newObject(3*xPoint, yPoint, xPoint-2, yPoint-2, square);
    newObject(3*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
    newObject(3*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
    newObject(3*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
    newObject(3*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

    newObject(5*xPoint, yPoint, xPoint-2, yPoint-2, square);
    newObject(5*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
    newObject(5*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
    newObject(5*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
    newObject(5*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

    newObject(7*xPoint, yPoint, xPoint-2, yPoint-2, square);
    newObject(7*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
    newObject(7*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
    newObject(7*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
    newObject(7*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

    newObject(9*xPoint, yPoint, xPoint-2, yPoint-2, square);
    newObject(9*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
    newObject(9*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
    newObject(9*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
    newObject(9*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

    newPlayer(1, 0, bomber, 0);
    
}

void BomberServer::onClientDisconnect(ClientConnection * client) {
  objects.clear();
  players.clear();
}

void BomberServer::onNewMessage(std::string msg) {

  if(msg.compare("leftKey")==0)
    {
      players[0].first.first.first += -xMove;
      movePlayer(0);      
      //can he move? (test collision and stuff...)
    }
  else if(msg.compare("rightKey")==0)
    {
      players[0].first.first.first += xMove;
      movePlayer(0);
    }
  else if(msg.compare("upKey")==0)
    {
      players[0].first.first.second += -yMove;
      movePlayer(0);
    }
  else if(msg.compare("downKey")==0)
    {
      players[0].first.first.second += yMove;
      movePlayer(0);
    }
}

void BomberServer::movePlayer(int index)
{
  stringstream str;
  str << "bomber_" << players[index].first.first.first << "_" <<players[index].first.first.second;
  msgs.push_back(str.str());
}

void BomberServer::newPlayer(int x, int y, objType type, int clientId)
{
    players.push_back(player(bomb(pair<int,int>(x,y),type),clientId));
    
    movePlayer(0);
}

void BomberServer::newObject(int x, int y, int w, int h, objType type)
{
  objects.push_back(gameObj(genObj(pair<int,int> (x,y), pair<int,int> (w, h)),type));
  stringstream str;
  if(type == square)
    {
      str << "sq_" << x << "_" << y << "_" << w << "_" << h;
    }
  msgs.push_back(str.str());
}
