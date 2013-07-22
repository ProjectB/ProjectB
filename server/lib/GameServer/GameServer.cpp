/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include "GameServer.hpp"

using namespace std;


GameServer::GameServer() {
	isRunning = false;
}


GameServer::~GameServer() {
	gs.fixedObjects.clear();
	gs.players.clear();

	cout << "delete" << std::endl;

	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
		if ((*it).second->isConnected())
			(*it).second->disconnect();
	}

	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
		delete ((*it).second);
		it = clients.erase(it);
	}
}


void GameServer::broadcast(string msg) {
    for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
        (*it).second->sendMsg(msg);
    }
}


void GameServer::start() {
	mainThread = thread([this] {this->run();});
	stepThread = thread([this] {this->step();});
}

void GameServer::stop() {
	isRunning = false;
    mainThread.join();
}

void GameServer::run() {
    isRunning = true;

    while (isRunning) {
    	// novos clients
    	while(!ConnServer::isClientQueueEmpty()) {
    		ClientConnection * client = ConnServer::clientQueue.pop();
    		clients[client->guid] = client;

    		thread clientThread = thread([this, client] {this->receiveClientMessages(client);});
    		clientThread.detach();

    		onClientConnect(client);
    	}

    	// novas msgs
    	while(!messageQueue.empty()) {
    		Message* m = messageQueue.pop();
    		onNewMessage(m);
    	}

    	// check clients
    	for(map<string, ClientConnection*>::iterator it = clients.begin(); it != clients.end(); it++) {
    		if(!(*it).second->isConnected()) {
    			onClientDisconnect((*it).second);
    			delete ((*it).second);
    			clients.erase((*it).first);
    		}
    	}
    }
}

void GameServer::receiveClientMessages(ClientConnection * client) {
    vector<Message> msgs;
    while (client->isConnected()) {
        client->receiveMsg(msgs);

        for (vector<Message>::iterator it = msgs.begin(); it != msgs.end(); it++) {
            messageQueue.push(&(*it));
            cout << "pushed: " << pushCounter << std::endl;
            if ((*it).getMessage().compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                client->disconnect();
            }
        }

        msgs.clear();
    }
}

/* Todas as funções a seguir rodam em um mesmo frame temporal, na seguinte ordem:
 *
 * 1. onClientConnect       - uma vez para cada cliente que se conectou nesse frame
 * 2. onNewMessage          - uma vez para cada mensagem recebida nesse frame
 * 3. onClientDisconnect    - uma vez para cada cliente que se conectou nesse frame
 *
 *
 * A função _step_ roda uma vez por frame temporal e independe das outras funções.
 * Isso garante um update constante para os clientes evitando lag spikes ou frame stutters.
 *
 */

void GameServer::onClientConnect(ClientConnection * client) {
	// client connected
	gs.players[client->guid] = GenObject(client->guid, bomber, 1, 0, BOMBER_HEIGHT, BOMBER_WIDTH);
	broadcast(gs.players[client->guid].getMsg(true));

	client->sendMsg(gs.generateDifStateMessage(false));
}

void GameServer::onClientDisconnect(ClientConnection * client) {
	// client disconnect
	broadcast(gs.players[client->guid].getMsg(true, true));

	gs.players.erase(client->guid);
}

void GameServer::onNewMessage(Message* m) {
	gs.update(m->getGuid(), m->getMessage());
}

void GameServer::step() {

	while(true)
	{
		if(!clients.empty()) {
			string msg = gs.generateDifStateMessage();

			if (msg.compare(SEPARATOR) != 0) {
				//cout << "step: " << msg << std::endl;
				broadcast(msg);
			}
		}

		this_thread::sleep_for(chrono::milliseconds(1000 / FPS));
	}
}




















