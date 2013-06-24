/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include "BomberServer.hpp"

using namespace std;

BomberServer::BomberServer(int port) :
        GameServer(port) {
}

BomberServer::~BomberServer() {
    gs.fixedObjects.clear();
    gs.players.clear();
}

/* Todas as funções a seguir rodam em um mesmo frame temporal, na seguinte ordem:
 *
 * 1. onClientConnect       - uma vez para cada cliente que se conectou nesse frame
 * 2. onNewMessage          - uma vez para cada mensagem recebida nesse frame
 * 3. onClientDisconnect    - uma vez para cada cliente que se conectou nesse frame
 * 4. step                  - uma única vez
 */

void BomberServer::onClientConnect(ClientConnection * client) {
    // client connected
  gs.players[client->guid] = GenObject(client->guid, bomber, 1, 0, BOMBER_HEIGHT, BOMBER_WIDTH);
  broadcast(gs.players[client->guid].getMsg(true));
  
  client->sendMsg(gs.generateDifStateMessage(false));
}

void BomberServer::onClientDisconnect(ClientConnection * client) {
    // client disconnect
    broadcast(gs.players[client->guid].getMsg(true, true));

    gs.players.erase(client->guid);
}

void BomberServer::onNewMessage(string guid, string msg) {
    gs.update(guid, msg);
}

void BomberServer::step() {
    this_thread::sleep_for(chrono::milliseconds(1000 / FPS));
    string msg = gs.generateDifStateMessage();

    if (msg.compare(SEPARATOR) != 0) {
        //cout << "step: " << msg << std::endl;
        broadcast(msg);
    }
}

