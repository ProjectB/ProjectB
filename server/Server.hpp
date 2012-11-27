/*
 * Server.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <atomic>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>

#include "MultithreadQueue.hpp"
#include "ClientConnection.hpp"
#include "lib/PracticalSocket.hpp"

class Server {
    std::thread mainThread;
    std::atomic<bool> isRunning;
    TCPServerSocket * servSocket;
	unsigned short port;
	void run();
public:
	Server(unsigned short port);
	void start();
	void stop();
	void log(std::string msg);
	MultithreadQueue<ClientConnection*> clientQueue;
};


#endif /* SERVER_H_ */
