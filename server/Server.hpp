/*
 * Server.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */

#ifndef SERVER_H_
#define SERVER_H_

class Server {
	unsigned short port;
public:
	Server(unsigned short port);
	void run();
};


#endif /* SERVER_H_ */
