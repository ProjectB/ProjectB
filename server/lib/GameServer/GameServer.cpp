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
	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it).second->isConnected())
			(*it).second->disconnect();
	}

	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
		delete ((*it).second);
		it = clients.erase(it);
	}
}

void GameServer::broadcast(string msg) {
    for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++)
        sendMessageToClient((*it).second->guid, msg);
}

void GameServer::sendMessageToClient(std::string clientGuid, std::string msg) {
	if(!msg.empty())
	{
		if(clients.find(clientGuid) != clients.end())
			clients[clientGuid]->sendMsg(msg);
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
    while (isRunning)
    {
    	// novos clients
    	while(!ConnServer::isClientQueueEmpty())
    	{
    		ClientConnection * client = ConnServer::clientQueue.pop();

    		thread clientThread = thread([this, client] {this->receiveClientMessages(client);});
    		clientThread.detach();

    		onClientConnect(client);
    	}

    	// novas msgs
    	while(!messageQueue.empty())
    	{
    		Message* m = messageQueue.pop();
    		onNewMessage(m);
    	}

    	// check clients
    	for(map<string, ClientConnection*>::iterator it = clients.begin(); it != clients.end(); it++)
    	{
    		if(!(*it).second->isConnected())
    		{
    			this->onClientDisconnect((*it).second);
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
	GenObject *gObj = gs.createPlayer(0, 0);
	client->guid = gObj->guid;
	this->clients[client->guid] = client;

	//TODO: should generate stats message, not diff
	//sendMessageToClient(client->guid, gObj.generateObjectActionMessage((ObjectAction)Add) + SEPARATOR);

	//sendMessageToClient(client->guid, gs.generateDifStateMessage());
	gs.objects[client->guid] = gObj;
}

void GameServer::onClientDisconnect(ClientConnection * client) {
	// client disconnect
	broadcast((*gs.objects[client->guid]).generateObjectActionMessage((ObjectAction)Delete));
	gs.objects.erase(client->guid);
}

void GameServer::onNewMessage(Message* m)
{
	gs.update(m->getGuid(), m->getMessage());
}

void GameServer::step()
{
	timeval begin, end;
	double elapsedTime;
	unsigned int million = 1000000;
	int sleepTime;

	while(isRunning)
	{
		gettimeofday(&begin, NULL);

		if(!clients.empty())
		{
			string msg = gs.generateDifStateMessage();
			msg += gs.updateNPObjects();

			if (!msg.empty()) {
				broadcast(msg);
			}
		}
		gettimeofday(&end, NULL);

		elapsedTime = ((end.tv_sec * million +  end.tv_usec) - (begin.tv_sec * million + begin.tv_usec)) / 1000;
		sleepTime = (int)((double)(1000/FPS) - elapsedTime);

		this_thread::sleep_for(chrono::milliseconds(sleepTime));
	}
}




















