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
    gs.fixedObjects.clear();
    gs.players.clear();
}

void BomberServer::step() {
    //server roda a 120fps
    this_thread::sleep_for(chrono::milliseconds(1000 / 30));
    //string msg  = gs.getMsg();
    //cout << msg << endl;
    //broadcast(msg);
}

void BomberServer::onClientConnect(ClientConnection * client) {
    stringstream sgsm;
    sgsm << "client " << client->id << " connected" << endl;
    broadcast(sgsm.str());

    client->sendMsg(gs.getMsg());
    //gs.players[client->guid] = GenObject(bomber, 1, 0, BLOCK_SIZE, BLOCK_SIZE);
}

void BomberServer::onClientDisconnect(ClientConnection * client) {
    stringstream sgsm;
    sgsm << "client " << client->id << " disconnected" << endl;
    broadcast(sgsm.str());

    map<string, GenObject>::iterator it = gs.players.find(client->guid);
    gs.players.erase(it);
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

