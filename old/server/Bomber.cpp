/*
 *
 * Bomber.cpp
 *
 */


#include "defines.hpp"


#define _BOMBER_DEBUG 0

Bomber::Bomber(gameQueue socks)
{
  p1.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p1.nickname = "Player 1";

  p2.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p2.nickname = "Player 2";

  p3.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p3.nickname = "Player 3";

  p4.socket = socks.socketList.front();
  socks.socketList.pop_front();
  p4.nickname = "Player 4";

  /* start game */
}

Bomber::~Bomber()
{
  delete &p1;
  delete &p2;
  delete &p3;
  delete &p4;
  
}

string Bomber::createMessage(string str)
{
  string msg;
  int length = str.length();

  msg.push_back('0'); //FIN bit... 0 means final message
  msg.push_back('0'); //RSV1
  msg.push_back('0'); //RSV2
  msg.push_back('0'); //RSV3
  //opcode.. 0x01 = text frame
  msg.push_back('0');
  msg.push_back('0');
  msg.push_back('0');
  msg.push_back('1');

  msg.push_back('1'); //mask..1 means no mask

  if(length < 126) 
    {
      msg.push_back(length & 0x40);
      msg.push_back(length & 0x20);
      msg.push_back(length & 0x10);
      msg.push_back(length & 0x8);
      msg.push_back(length & 0x4);
      msg.push_back(length & 0x2);
      msg.push_back(length & 0x1);
    }
  else if(length < 65536)
    {

      //126
      msg.push_back('1');
      msg.push_back('1');
      msg.push_back('1');
      msg.push_back('1');
      msg.push_back('1');
      msg.push_back('1');
      msg.push_back('0');

      msg.push_back(length & 0x8000);
      msg.push_back(length & 0x4000);
      msg.push_back(length & 0x2000);
      msg.push_back(length & 0x1000);
      msg.push_back(length & 0x800);
      msg.push_back(length & 0x400);
      msg.push_back(length & 0x200);
      msg.push_back(length & 0x100);
      msg.push_back(length & 0x80);
      msg.push_back(length & 0x40);
      msg.push_back(length & 0x20);
      msg.push_back(length & 0x10);
      msg.push_back(length & 0x8);
      msg.push_back(length & 0x4);
      msg.push_back(length & 0x2);
      msg.push_back(length & 0x1);
    }

  for(int i=0; i < length; i++)
    {
      msg.push_back(str.at(i));
    }

  return msg;
  
}
  
  
  


  
  
}


