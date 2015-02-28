/*
 * GameServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef GAMESERVER_HPP_
#define GAMESERVER_HPP_

#include <iostream>
#include <sstream>
#include <sys/time.h>
#include "../defs.hpp"
#include "Game.hpp"
#include "../ConnectionManager/ClientConnection.hpp"
#include "../ConnectionManager/Message.hpp"
#include "../ConnectionManager/ConnServer.hpp"

class GameServer {
public:

	static GameServer& getInstance()
	{
		static GameServer instance;
		return instance;
	}
	void initGameServer();

    void start();
    void stop();


    void sendMessageToClientArray(std::vector<std::string>, string);
private:
    ~GameServer();
    GameServer() {};

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
	GameServer(GameServer const&)               = delete;
	void operator=(GameServer const&)  = delete;


    std::thread mainThread;
    unsigned int gamesCounter;
    std::map<unsigned int, Game*> arrayGames;
    std::map<std::string, ClientConnection*> clients;
    std::map<std::string, unsigned int> mapClientGame;
    MultithreadQueue<Message*> messageQueue;

    bool isRunning;

    Game* getClientGameObject(std::string);
    void receiveClientMessages(ClientConnection *);
    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(Message* m);
    void sendMessageToClient(std::string clientGuid, std::string msg);
    void run();
};

#endif /* GAMESERVER_HPP_ */
