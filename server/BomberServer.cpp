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

}
