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
    if (answerWSClient(msg.str())) {
        cout << "Connection established" << endl;
    } else {
        cout << "Fail to answer client" << endl;
        delete sock;
        this->sock = NULL;
    }
}

ClientConnection::~ClientConnection() {
    if (sock != NULL)
        delete sock;
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
        if ((*i).compare("Sec-WebSocket-Key:") == 0)
            found = true;
    }
    if (!found)
        return false;

    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11\0";

    //cout << "key: " << key << endl;

    unsigned char answer[256];
    for (int i = 0; i < 256; i++)
        answer[i] = 0;
    sha1::calc((const void *) key.c_str(), (unsigned int) key.length(), answer);

    //cout << "sha1: " << answer << endl;
    //cout << "base64: " << base64::encode(answer, 20) << endl;

    string httpAnswer = "HTTP/1.1 101 Switching Protocols\r\n"
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

void ClientConnection::updateRcv(unsigned int& pos, void *buffer, bool block) {
    if (pos + 1 == RCVBUFSIZE) {
        memset(buffer, 0, RCVBUFSIZE);
        if (block)
            this->sock->recv(buffer, RCVBUFSIZE);
        else if (this->hasData())
            this->sock->recv(buffer, RCVBUFSIZE);
    }
    pos = (pos + 1) % RCVBUFSIZE;
//    printf("buffer[%d]=%x\n", pos, ((unsigned char *)buffer)[pos]);
}

/* receive packet from client's browser */
void ClientConnection::receiveMsg(vector<string>& msgs) {
    unsigned char buffer[RCVBUFSIZE];
    unsigned int pos;

    string message;
    unsigned char firstByte;
    unsigned char secondByte;
    unsigned char mask[4];
    int payloadLen;
    bool boolMask;
    bool finalMessage;
    bool textFrame;
    bool needContinuation;

    cout << endl << endl;

    needContinuation = false;
    pos = RCVBUFSIZE - 1;
    updateRcv(pos, buffer);
    while (true) {

        firstByte = buffer[pos];
        updateRcv(pos, buffer);
        secondByte = buffer[pos];

        if (_BYTES)
            cout << "0x" << hex << (unsigned int) firstByte << " 0x" << (unsigned int) secondByte << endl;

        if ((firstByte & 0x70) != 0)
            return; // invalid frame

        // FIN
        if (firstByte < 0x80)
            finalMessage = false;
        else
            finalMessage = true;

        // 4 bits da direita do firstByte
        if ((firstByte & 0xF) == 0) {
            if (!needContinuation)
                return; //invalid

        } else if ((firstByte & 0xF) == 0x8) {
            //connection close
            msgs.push_back("_0x8_connection_close");
            return;
        } else if ((firstByte & 0xF) == 0x1)
            textFrame = true;
        else
            textFrame = false;

        if (secondByte < 0x80)
            boolMask = false;
        else
            boolMask = true;

        // PAYLOAD
        payloadLen = (int) (secondByte & 0x7F);

        if (payloadLen < 126) {
            //do nothing
            cout << "payload < 126: " << dec << payloadLen << endl;
        } else if (payloadLen == 126) {
            for (int i = 0; i < 2; i++) {
                updateRcv(pos, buffer);
                payloadLen += buffer[pos];
            }
            cout << "payload == 126: " << dec << payloadLen << endl;
        } else { // (payloadLen == 127)
            for (int k = 0; k < 8; k++) {
                updateRcv(pos, buffer);
                payloadLen += buffer[pos];
            }
            cout << "payload > 126: " << dec << payloadLen << endl;
        }

        if (boolMask) {
            for (int i = 0; i < 4; i++) {
                updateRcv(pos, buffer);
                mask[i] = buffer[pos];
            }
        }

        for (int i = 0; i < payloadLen; i++) {
            updateRcv(pos, buffer);
            char c = (boolMask == true) ? (buffer[pos] ^ mask[i % 4]) : buffer[pos];
            message.push_back(c);
        }

        cout << "msg: " << message << endl;

        if (finalMessage) {
            needContinuation = false;
            if (textFrame)
                msgs.push_back(message);
            else
                cout << "invalid:" << message << endl;
            message.clear();
        } else
            needContinuation = true;

        updateRcv(pos, buffer, false);
    }
}

string ClientConnection::createPacket(string str) {
    vector<unsigned char> msg;
    int length = str.length();

    msg.push_back(0x81);
    if (length < 126)
        msg.push_back(length);
    else {
        msg.push_back(126);
        msg.push_back(length);
    }

    for (int i = 0; i < length; i++)
        msg.push_back(str[i]);

    return string(msg.begin(), msg.end());
}

int ClientConnection::hasData(int sec, int usec) {
    return sock->hasData(sec, usec);
}
