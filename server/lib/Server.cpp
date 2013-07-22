/*
 * Server.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#include "Server.hpp"

Server::Server(unsigned short port) {
    servSocket = new  TCPServerSocket(port);
    this->port = port;
}

Server::~Server() {
	delete servSocket;
}

