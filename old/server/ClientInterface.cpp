/*
 * clientInterface.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: ricardo
 */


#include "defines.hpp"


#define _SWSSDEBUG 0

ClientInterface::ClientInterface(TCPSocket *sock)
{
  //  GameServer gs;
  string message;
  
  socket = sock;

  isQueued = true;
  //gs.joinGameQueue(sock);

  /* not very smart :D */
  while(isQueued);

  
  while(1)
    {
      message = receivePacket();
      
      if(message.compare("_0x8_connection_close") == 0)
	{
	  //cout << "connection closed!" << endl;
	  break;
	}
      else if(message.compare("_0x9_ping") == 0)
	{
	}
      else if(message.compare("_0xA_pong") == 0)
	{
	}
      else
	{
	  sendToServer(message);
	}
    }
}

ClientInterface::~ClientInterface()
{
  delete socket;
}

void ClientInterface::unqueue()
{
  isQueued = false;
}

/* send client's input to server */
void ClientInterface::sendToServer(string message)
{
return;
}

/* receive packet from client's browser */
string ClientInterface::receivePacket()
{
  char buf[RCVBUFSIZE];
  
  socket->recv(buf, RCVBUFSIZE);

  return translatePacket(buf);
}

string ClientInterface::translatePacket(char buffer[RCVBUFSIZE])
{

  string message;
  char firstByte = buffer[0];
  char secondByte = buffer[1];
  char mask[4];
  int payloadLen;
  int maskOffset; //payload data starting byte may change because of payload length
  bool boolMask;
  bool finalMessage;

  //.......SORCERY
  if(firstByte < 0x80)
    {
      if(_SWSSDEBUG) cout << "finalMessage false" << endl;
      //not the final message
      finalMessage = true;
    }
  else 
    {
      if(_SWSSDEBUG) cout << "finalMessage true" << endl;
      finalMessage = false;
    }

  //primeiros 4 bits
  if((firstByte & 0xF) == 0)
    {
      if(_SWSSDEBUG) cout << "continuation frame" << endl;
      //continuation frame
    }
  else if((firstByte & 0xF) == 0x1) 
    {
      if(_SWSSDEBUG) cout << "text frame" << endl;
      //text frame
    }
  else if((firstByte & 0xF) == 0x8)
    {
      //connection close
      if(_SWSSDEBUG) cout << "connection closed" << endl;
      return "_0x8_connection_close";
    }
  else if((firstByte & 0xF) == 0x9)
    {
      //ping
      if(_SWSSDEBUG) cout << "ping" << endl;
      return "_0x9_ping";
    }
  else if((firstByte & 0xF) == 0xA) 
    {
      //pong
      if(_SWSSDEBUG) cout << "pong" << endl;
      return "_0xA_pong";
    }


  /* SORCERY, BE CAREFUL: IF FALSE THEN TRUE :D */
  if(secondByte < 0x80)
    {
      //not masked
      if(_SWSSDEBUG) cout << "boolMask false" << endl;
      boolMask = true;
    }
  else
    {
      if(_SWSSDEBUG) cout << "boolMask true" << endl;
      //masked
      boolMask = false;
    }

  payloadLen = (int)(secondByte & 0x7F);
  if(_SWSSDEBUG) cout << "(1st) payload length: " << payloadLen << endl;


  if(payloadLen == 126)
    {
      if(_SWSSDEBUG) cout << "(2nd-126) payload length: " << payloadLen << endl;
      payloadLen += buffer[2] + buffer[3];

      if(boolMask)
	{
	  mask[0] = buffer[4];
	  mask[1] = buffer[5];
	  mask[2] = buffer[6];
	  mask[3] = buffer[7];
	  maskOffset = 8;
	}
      else
	{
	  maskOffset = 4;
	}
      if(_SWSSDEBUG) cout << "out: " << maskOffset << endl;
    }
  else if(payloadLen == 127)
    {
      if(_SWSSDEBUG) cout << "(2nd-127) payload length: " << payloadLen << endl;
      payloadLen += buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7] + buffer[8] + buffer[9];
      
      if(boolMask)
	{
	  mask[0] = buffer[10];
	  mask[1] = buffer[11];
	  mask[2] = buffer[12];
	  mask[3] = buffer[13];
	  maskOffset = 14;
	}
      else
	{
	  maskOffset = 10;
	}
      if(_SWSSDEBUG) cout << "out: " << maskOffset << endl;
    }
  else
    {
      //payload length < 126
      if(boolMask)
	{
	  mask[0] = buffer[2];
	  mask[1] = buffer[3];
	  mask[2] = buffer[4];
	  mask[3] = buffer[5];
	  maskOffset = 6;
	}
      else
	{
	  maskOffset = 2;
	}
      if(_SWSSDEBUG) cout << "no changes..out: " << maskOffset << endl;
    }

  int i;

  if(boolMask)
    {
      for(i=0; i < payloadLen; i++)
	{
	  message.push_back(buffer[i+maskOffset] ^ mask[i % 4]);
	}
    }
  else
    {
      if(_SWSSDEBUG) cout << "intern...: ";
      for(i=0; i < payloadLen; i++)
	{
	  if(_SWSSDEBUG) cout << buffer[i+maskOffset];
	  message.push_back(buffer[i+maskOffset]);
	}
      if(_SWSSDEBUG) cout << endl;
    }

  if(_SWSSDEBUG) cout << "this message: " << message << endl;

 
  if(!finalMessage)
    {
      //get next message?
      message += receivePacket();
    }

  if(_SWSSDEBUG) cout << "return" << endl;
 
  return message;
}

