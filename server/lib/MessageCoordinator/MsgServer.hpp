/*
 * GameServer.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef MSGSERVER_H_
#define MSGSERVER_H_

#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <cstring>
#include <ctime>

#include "../MultithreadQueue.hpp"
#include "Message.hpp"
#include "../Logger.hpp"
#include "ClientConnection.hpp"

class MsgServer: public Logger, public Server{
private:
	std::thread recvThread;
	std::thread sendThread;
	MultithreadQueue<Message*> messageQueue;
	std::string serverStartTime;
	std::atomic<bool> isRunning;
	unsigned long messagesProccessed;
	int hasDataSec;
	int hasDataUSec;

public:

	MsgServer();
	MsgServer(unsigned int port);
	virtual ~MsgServer();

	void start();
	void stop();

protected:
	void broadcast(std::string msg);
	void receiveMessages();
	void processQueue();

	Server server;
};

#endif /* MSGSERVER_H_ */
