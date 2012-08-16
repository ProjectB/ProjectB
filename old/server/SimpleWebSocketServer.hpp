#include <stdio.h>
<<<<<<< HEAD
#include <iostream>
#include <cstdlib>
=======
#include <iostream>              // For cerr and cout
#include <cstdlib>               // For atoi()
>>>>>>> kodo
#include <cstring>
#include <sstream>
#include <vector>
#include <string>

<<<<<<< HEAD
=======
#include "PracticalSocket.h"     // For Socket, ServerSocket, and SocketException
#include "SHA1.h"                // For SHA-1 encoding
#include "base64.h"


>>>>>>> kodo
#ifndef SWSS_HPP_
#define SWSS_HPP_

using namespace std;

<<<<<<< HEAD
const unsigned int RCVBUFSIZE = 32;


namespace SimpleWebSocketServer
=======
const unsigned int RCVBUFSIZE = 32;        // Size of receive buffer


namespace Server
>>>>>>> kodo
{
  class SimpleWebSocketServer
  {
  private:
    inline const unsigned int rol(const unsigned int, const unsigned int);
<<<<<<< HEAD
    void innerHash(unsigned int, unsigned int);
  public:
=======
    void innerHash(unsigned int *, unsigned int *);
    void calc(const void *, const int, unsigned char *);
    void toHexString(const unsigned char *, char *);
    void handleTCPClient(TCPSocket *sock);
    string getMessage(TCPSocket *sock);
    string translateMessage(char *, TCPSocket *sock);
  public:
    void runServer(int, char **);
  };
}
>>>>>>> kodo

#endif /* SWSS_HPP_ */
