#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "lib/PracticalSocket.hpp"
#include "ClientConnection.hpp"
#include "lib/sha1.hpp"
#include "lib/base64.hpp"
#include "defs.hpp"

using namespace std;

ClientConnection::ClientConnection(int id, TCPSocket* sock) {
    this->id = id;
    this->sock = sock;

    cout << "Handling client ";
    try {
        this->address = sock->getForeignAddress();
        cout << this->address << ":";
    } catch (const SocketException& e) {
        cerr << "Unable to get foreign address" << endl;
        this->address = "undefined";
    }
    try {
        this->port = sock->getForeignPort();
        cout << this->port;
    } catch (const SocketException& e) {
        cerr << "Unable to get foreign port" << endl;
        this->port = 0;
    }
    cout << endl;

    char recvBuffer[RCVBUFSIZE];
    unsigned int recvMsgSize;
    stringstream msg;

    while ((recvMsgSize = sock->recv(recvBuffer, RCVBUFSIZE)) > 0) {
        int pos;
        if ((pos = string(recvBuffer).find("\r\n\r\n")) != string::npos) {
            recvBuffer[pos] = 0;
            msg << recvBuffer;
            break;
        }
        msg << recvBuffer;
    }

    cout << "Answering client" << endl;
    if(answerWSClient(msg.str())) {
        cout << "Connection established" << endl;
    }
    else {
        cout << "Fail to answer client" << endl;
        delete sock;
        this->sock = NULL;
    }
}

ClientConnection::~ClientConnection() {
    if(sock != NULL) delete sock;
}

bool ClientConnection::isConnected() {
    bool isConnected;
    connMutex.lock();
    isConnected = !(sock == NULL);
    connMutex.unlock();
    return isConnected;
}

void ClientConnection::disconnect() {
    connMutex.lock();
    delete sock;
    sock = NULL;
    connMutex.unlock();
}

string ClientConnection::str() {
    stringstream ss("");
    ss << address << ":" << port;
    return ss.str();
}

bool ClientConnection::answerWSClient(string msg) {
    string key;
    vector<string> lines;
    istringstream iss(msg);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string>>(lines));

    bool found = false;
    for (vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
        if (found) {
            key = *i;
            break;
        }
        if ((*i).compare("Sec-WebSocket-Key:") == 0) found = true;
    }
    if(!found) return false;

    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11\0";

    //cout << "key: " << key << endl;

    unsigned char answer[256];
    for(int i = 0; i < 256; i++) answer[i] = 0;
    sha1::calc((const void *)key.c_str(), (unsigned int)key.length(), answer);

    //cout << "sha1: " << answer << endl;
    //cout << "base64: " << base64::encode(answer, 20) << endl;

    string httpAnswer =
      "HTTP/1.1 101 Switching Protocols\r\n"
      "Upgrade: websocket\r\n"
      "Connection: Upgrade\r\n"
      "Sec-WebSocket-Accept: " + base64::encode(answer, 20) + "\r\n\r\n";

    this->sock->send(httpAnswer.c_str(), strlen(httpAnswer.c_str()));

    return true;
}

void ClientConnection::sendMsg(string message) {
	string msg = createPacket(message);
    this->sock->send(msg.c_str(), strlen(msg.c_str()));
}

/* receive packet from client's browser */
string ClientConnection::receiveMsg()
{
  char buf[RCVBUFSIZE];
  this->sock->recv(buf, RCVBUFSIZE);
  return translatePacket(buf);
}

//by kodo!
string ClientConnection::translatePacket(char buffer[RCVBUFSIZE])
{

  string message;
  unsigned char firstByte = buffer[0];
  unsigned char secondByte = buffer[1];
  unsigned char mask[4];
  int payloadLen;
  int maskOffset; //payload data starting byte may change because of payload length
  bool boolMask;
  bool finalMessage;

  if(_BYTES)
    {
      cout << "Bytes: " << hex << (unsigned int)firstByte << " and " << (unsigned int)secondByte << endl;
    }

  if(firstByte < 0x80)
    {
      if(_SWSSDEBUG) cout << "finalMessage false" << endl;
      //not the final message
      finalMessage = false;
    }
  else
    {
      if(_SWSSDEBUG) cout << "finalMessage true" << endl;
      finalMessage = true;
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
  else
    {
      if(_SWSSDEBUG) cout << "control: " << (unsigned int)(firstByte & 0xF) << endl;
      return "control frame";
    }


  if(secondByte < 0x80)
    {
      //not masked
      if(_SWSSDEBUG) cout << "boolMask false" << endl;
      boolMask = false;
    }
  else
    {
      if(_SWSSDEBUG) cout << "boolMask true" << endl;
      //masked
      boolMask = true;
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
      message += receiveMsg();
    }

  if(_SWSSDEBUG) cout << "return" << endl;

  return message;
}

/*
unsigned char createByte(const string byteStr) {
    unsigned char byte = 0;
    if (byteStr.length() < 8) return 0;

    if (byteStr[0] == '1') byte |= 0x80;
    if (byteStr[0] == '1') byte |= 0x40;
    if (byteStr[0] == '1') byte |= 0x20;
    if (byteStr[0] == '1') byte |= 0x10;
    if (byteStr[0] == '1') byte |= 0x8;
    if (byteStr[0] == '1') byte |= 0x4;
    if (byteStr[0] == '1') byte |= 0x2;
    if (byteStr[0] == '1') byte |= 0x1;

    return byte;
}
*/

string ClientConnection::createPacket(string str)
{
  vector<unsigned char> msg;
  int length = str.length();

  /* hello
  msg.push_back(0x81);
  msg.push_back(0x05);
  msg.push_back(0x48);
  msg.push_back(0x65);
  msg.push_back(0x6c);
  msg.push_back(0x6c);
  msg.push_back(0x6f);
  */

  msg.push_back(0x81);
  if (length < 126)
      msg.push_back(length);
  else {
      msg.push_back(126);
      msg.push_back(length);
  }

  for(int i=0; i < length; i++) msg.push_back(str[i]);

  return string(msg.begin(), msg.end());
}

int ClientConnection::hasData() {
    return sock->hasData();
}
