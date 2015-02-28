/*
 * BomberServer.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#include "GameServer.hpp"

using namespace std;

GameServer::~GameServer()
{
	for (map<string, ClientConnection *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if ((*it).second->isConnected())
			(*it).second->disconnect();
	}

	for (map<string, ClientConnection *>::iterator it = this->clients.begin(); it != this->clients.end();) {
		delete ((*it).second);
		it = this->clients.erase(it);
	}
}

void GameServer::initGameServer()
{
	this->isRunning = false;
	this->gamesCounter = 0;
	this->start();
}

void GameServer::sendMessageToClientArray(std::vector<std::string> arrayClientsGuid, string msg)
{
	for(unsigned int i=0; i < arrayClientsGuid.size(); i++)
	{
		this->sendMessageToClient(arrayClientsGuid[i], msg);
	}
}

void GameServer::sendMessageToClient(std::string clientGuid, std::string msg)
{
	if(!msg.empty())
	{
		if(this->clients.find(clientGuid) != this->clients.end())
			this->clients[clientGuid]->sendMsg(msg);
	}
}

Game* GameServer::getClientGameObject(std::string clientGuid)
{
	if(this->mapClientGame.find(clientGuid) != this->mapClientGame.end())
	{
		unsigned int gameIndex = this->mapClientGame[clientGuid];
		if(this->arrayGames.find(gameIndex) != this->arrayGames.end())
		{
			return this->arrayGames[gameIndex];
		}
	}
	return nullptr;
}

void GameServer::start()
{
	isRunning = true;
	mainThread = thread([this] {this->run();});
}

void GameServer::stop()
{
	isRunning = false;
    mainThread.join();
    delete this;
}

void GameServer::run()
{
    while (isRunning)
    {
    	// novos clients
    	while(!ConnServer::isClientQueueEmpty())
    	{
    		ClientConnection * client = ConnServer::clientQueue.pop();

    		thread clientThread = thread([this, client] {this->receiveClientMessages(client);});
    		clientThread.detach();

    		this->onClientConnect(client);
    	}

    	// novas msgs
    	while(!this->messageQueue.empty())
    	{
    		Message* m = this->messageQueue.pop();
    		this->onNewMessage(m);
    	}

    	// check clients
    	for(map<string, ClientConnection*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    	{
    		if(!(*it).second->isConnected())
    		{
    			this->onClientDisconnect((*it).second);
    			delete ((*it).second);
    			this->clients.erase((*it).first);
    		}
    	}
    }
}

void GameServer::receiveClientMessages(ClientConnection * client)
{
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

void GameServer::onClientConnect(ClientConnection * client)
{
	// client connected
	//TODO: should check if a new Game should be created... for now allocating a new Game for every client
	Game *g = new Game();
	this->arrayGames[this->gamesCounter] = g;
	GenObject *gObj = g->createPlayer(0, 0);
	client->guid = gObj->guid;
	this->clients[client->guid] = client;
	this->mapClientGame[client->guid] = this->gamesCounter;
	this->gamesCounter++;
	//TODO: should generate stats message, not diff
	//sendMessageToClient(client->guid, gObj.generateObjectActionMessage((ObjectAction)Add) + SEPARATOR);

	//sendMessageToClient(client->guid, gs.generateDifStateMessage());
}

void GameServer::onClientDisconnect(ClientConnection * client)
{
	// client disconnect
	Game *g = this->getClientGameObject(client->guid);
	if(g != nullptr)
	{
		this->sendMessageToClientArray(g->getAllGuids(), g->deletePlayer(client->guid));
		if(g->getNumPlayersOnGame() == 0)
		{
			//if no players exist in the game, delete it
			this->arrayGames.erase(this->mapClientGame[client->guid]);
			//TODO: a lock should be necessary here
			delete g;
		}

		//remove client from <Client,Game> map
		this->mapClientGame.erase(client->guid);
	}
}

void GameServer::onNewMessage(Message* m)
{
	std::string guid = m->getGuid();
	Game *g = this->getClientGameObject(guid);
	if(g != nullptr)
	{
		g->update(guid, m->getMessage());
	}
}
