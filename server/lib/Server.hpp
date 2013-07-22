/*
 * Server.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Logger.hpp"
#include "util/PracticalSocket.hpp"

class Server {
private:
    TCPServerSocket * servSocket;
    unsigned short port;
public:
    Server(unsigned short port);
    ~Server();
};


#endif /* SERVER_HPP_ */
