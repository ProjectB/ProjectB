/*
 * Server.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_


class Server : public Logger {
private:
    TCPServerSocket * servSocket;
    unsigned short port;
public:
    Server(unsigned short port);
    ~Server();
};


#endif /* SERVER_HPP_ */
