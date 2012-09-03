/*
 * ClientConnection.h
 *
 *  Created on: Aug 11, 2012
 *      Author: ricardo
 */
#include <string>
#include "lib/PracticalSocket.h"


#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

class ClientConnection {
public:
	ClientConnection(TCPSocket* sock);
private:
	void answerWSClient(TCPSocket* sock, string msg);
};


#endif /* CLIENTCONNECTION_H_ */
