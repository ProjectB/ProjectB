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
	isRunning = true;
	mainThread = thread([this] {this->run();});
	stepThread = thread([this] {this->step();});
}

void GameServer::stop() {
	isRunning = false;
    mainThread.join();
    stepThread.join();
}

void GameServer::run() {
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
            if ((*it).getMessage().compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                client->disconnect();
            }
            else
            {
            	messageQueue.push(&(*it));
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
	GenObject gObj = GenObject(client->guid, Bomber, 1, 0, BOMBER_HEIGHT, BOMBER_WIDTH);

	client->sendMsg(gObj.generateObjectActionMessage((ObjectAction)Add));

	client->sendMsg(gs.generateDifStateMessage(true));
	gs.players[client->guid] = gObj;
}

void GameServer::onClientDisconnect(ClientConnection * client) {
	// client disconnect
	broadcast(gs.players[client->guid].generateObjectActionMessage((ObjectAction)Delete));

	gs.players.erase(client->guid);
}

void GameServer::onNewMessage(Message* m) {
	gs.update(m->getGuid(), m->getMessage());
}

void GameServer::step() {
	while(isRunning)
	{
		if(!clients.empty()) {
			string msg = gs.generateDifStateMessage(false);

			if (!msg.empty()) {
				broadcast(msg);
			}
		}

		this_thread::sleep_for(chrono::milliseconds(1000 / FPS));
	}
}




















