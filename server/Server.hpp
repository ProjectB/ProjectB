/*
 * Server.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <atomic>
#include <thread>
#include "BomberServer.hpp"
#include "lib/PracticalSocket.hpp"

class Server {
    std::thread mainThread;
    std::atomic<bool> isRunning;
    BomberServer * bs;
    TCPServerSocket * servSocket;
	unsigned short port;
	void run();
public:
	Server(unsigned short port);
	void start();
	void stop();
	void log(std::string msg);
};


#endif /* SERVER_H_ */
