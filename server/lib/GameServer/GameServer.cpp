/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include "GameServer.hpp"

using namespace std;

/*
GameServer::GameServer(int port) :
		ConnServer(port) {
}
*/

/*
GameServer::~GameServer() {
	gs.fixedObjects.clear();
	gs.players.clear();
}
*/

/* Todas as funções a seguir rodam em um mesmo frame temporal, na seguinte ordem:
 *
 * 1. onClientConnect       - uma vez para cada cliente que se conectou nesse frame
 * 2. onNewMessage          - uma vez para cada mensagem recebida nesse frame
 * 3. onClientDisconnect    - uma vez para cada cliente que se conectou nesse frame
 * 4. step                  - uma única vez
 */

void GameServer::onClientConnect(ClientConnection * client) {
	// client connected
	gs.players[client->guid] = GenObject(client->guid, bomber, 1, 0, BOMBER_HEIGHT, BOMBER_WIDTH);
	//broadcast(gs.players[client->guid].getMsg(true));

	client->sendMsg(gs.generateDifStateMessage(false));
}

void GameServer::onClientDisconnect(ClientConnection * client) {
	// client disconnect
	//broadcast(gs.players[client->guid].getMsg(true, true));

	gs.players.erase(client->guid);
}

void GameServer::onNewMessage(string guid, string msg) {
	gs.update(guid, msg);
}

/*
void GameServer::step() {
	this_thread::sleep_for(chrono::milliseconds(1000 / FPS));
	string msg = gs.generateDifStateMessage();

	if (msg.compare(SEPARATOR) != 0) {
		//cout << "step: " << msg << std::endl;
		broadcast(msg);
	}
}
*/
