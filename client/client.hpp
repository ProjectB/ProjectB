//============================================================================
// Name        : client.hpp
// Author      : 
// Version     :
// Copyright   : 
// Description : client .h! :D
//============================================================================

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

namespace Websocket
{
  class WebSocketClient
  {
  private:
    int intPorta;
    bool boolExecutando;

    void error(const char *msg);
  public:
    bool isBoolExecutando();
    int getIntPorta();
    void setIntPorta(int intPorta);
  }
}

#endif /* CLIENT_HPP_ */

