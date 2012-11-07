/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include <iostream>
#include <sstream>
#include "BomberServer.hpp"

#define FPS 30

using namespace std;

BomberServer::BomberServer() {
}

BomberServer::~BomberServer() {
    gs.fixedObjects.clear();
    gs.players.clear();
}

void BomberServer::step() {
    this_thread::sleep_for(chrono::milliseconds(1000 / FPS));    
    string msg  = gs.getMsg(false);

    
    //cout << msg << endl;
    if(msg.compare("|") != 0)
      broadcast(msg);
}

void BomberServer::onClientConnect(ClientConnection * client) {
    stringstream sgsm;
    sgsm << "client " << client->id << " connected" << endl;
    broadcast(sgsm.str());

    gs.players[client->guid] = GenObject(client->guid, bomber, 1, 0, BLOCK_SIZE, BLOCK_SIZE);
    client->sendMsg(gs.getMsg(true));
}

void BomberServer::onClientDisconnect(ClientConnection * client) {
    stringstream sgsm;
    sgsm << "client " << client->id << " disconnected" << endl;
    broadcast(sgsm.str());

    gs.players.erase(client->guid);
}

void BomberServer::onNewMessage(string guid, string msg) {
    if (msg.compare("leftKey") == 0)
        gs.players[guid].x -= gs.xMove;
    else if (msg.compare("rightKey") == 0)
        gs.players[guid].x += gs.xMove;
    else if (msg.compare("upKey") == 0)
        gs.players[guid].y -= gs.yMove;
    else if (msg.compare("downKey") == 0)
        gs.players[guid].y += gs.yMove;
}
