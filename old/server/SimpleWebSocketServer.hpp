#include <stdio.h>
#include <iostream>              // For cerr and cout
#include <cstdlib>               // For atoi()
#include <cstring>
#include <sstream>
#include <vector>
#include <string>


#include "PracticalSocket.h"     // For Socket, ServerSocket, and SocketException
#include "SHA1.h"                // For SHA-1 encoding
#include "base64.h"


#ifndef SWSS_HPP_
#define SWSS_HPP_

using namespace std;


const unsigned int RCVBUFSIZE = 32;        // Size of receive buffer

namespace Server
{
  class SimpleWebSocketServer
  {
  private:
    inline const unsigned int rol(const unsigned int, const unsigned int);
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

#endif /* SWSS_HPP_ */
