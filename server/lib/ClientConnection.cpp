#include "defs.hpp"
#include "util/PracticalSocket.hpp"
#include "util/sha1.hpp"
#include "util/base64.hpp"
#include "ClientConnection.hpp"

using namespace std;

ClientConnection::ClientConnection(int id, TCPSocket* sock) {
    this->id = id;
    this->sock = sock;

    try {
        this->address = sock->getForeignAddress();
    } catch (const SocketException& e) {
        this->address = "undefined";
        if(_CLIENT_ERR_DEBUG) log("Unable to get foreign address");
    }
    try {
        this->port = sock->getForeignPort();
    } catch (const SocketException& e) {
        this->port = 0;
        if(_CLIENT_ERR_DEBUG) log("Unable to get foreign port");
    }
    if(_CLIENT_DEBUG) {
        stringstream debug;
        debug << "Handling client " << this->str();
        log(debug.str());
    }

    stringstream ssGuid;
    ssGuid << this->address << this->port << this->id;
    this->guid = ssGuid.str();

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

    if (answerWSClient(msg.str())) {
        if(_CLIENT_DEBUG) log("Connection established");
    } else {
        if(_CLIENT_ERR_DEBUG) log("Fail to answer client");
        disconnect();
    }
}

ClientConnection::~ClientConnection() {
    //if(_CLIENT_DEBUG) log("Client disposed");
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
    if(_CLIENT_DEBUG) log("Connection finished");
}

string ClientConnection::str() {
    stringstream ss;
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

    unsigned char answer[256];
    for (int i = 0; i < 256; i++)
        answer[i] = 0;
    sha1::calc((const void *) key.c_str(), (unsigned int) key.length(), answer);

    string httpAnswer = "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: " + base64::encode(answer, 20) + "\r\n\r\n";

    this->sock->send(httpAnswer.c_str(), strlen(httpAnswer.c_str()));

    return true;
}

void ClientConnection::sendMsg(string message) {
    string msg = createPacket(message);
    if (this->isConnected())
        this->sock->send(msg.c_str(), strlen(msg.c_str()));
}

void ClientConnection::updateRcv(unsigned int& pos, void *buffer, bool block) {
    pos = (pos + 1) % RCVBUFSIZE;

    if (pos == 0) {
        memset(buffer, 0, RCVBUFSIZE);
        if (block)
            this->sock->recv(buffer, RCVBUFSIZE);
        else if (this->hasData())
            this->sock->recv(buffer, RCVBUFSIZE);
    }
}

/* receive and translate packet from client's browser */
void ClientConnection::receiveMsg(vector<string>& msgs) {
    unsigned char buffer[RCVBUFSIZE];
    unsigned int pos;

    string message;
    stringstream debugMsg;
    unsigned char firstByte;
    unsigned char secondByte;
    unsigned char mask[4];
    int payloadLen;
    bool boolMask;
    bool finalMessage;
    bool textFrame;
    bool needContinuation;

    needContinuation = false;
    pos = -1;
    updateRcv(pos, buffer);
    while (true) {

        firstByte = buffer[pos];
        updateRcv(pos, buffer);
        secondByte = buffer[pos];

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
        else {
            if (_CLIENT_ERR_DEBUG) {
                stringstream debug;
                debug << "INVALID_FRAME_TYPE:" << hex << (firstByte & 0xF);
                log(debug.str());
            }
            textFrame = false;
        }

        if (secondByte < 0x80)
            boolMask = false;
        else
            boolMask = true;

        // PAYLOAD
        payloadLen = (int) (secondByte & 0x7F);

        if (payloadLen < 126) { //do nothing
        } else if (payloadLen == 126)
            for (int i = 0; i < 2; i++) {
                updateRcv(pos, buffer);
                payloadLen += buffer[pos];
            }
        else
            // (payloadLen == 127)
            for (int k = 0; k < 8; k++) {
                updateRcv(pos, buffer);
                payloadLen += buffer[pos];
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

        if (finalMessage) {
            needContinuation = false;
            if (textFrame)
                msgs.push_back(message);
            if (_CLIENT_MSG_DEBUG) {
                debugMsg << "[0x" << hex << (unsigned int) firstByte << " 0x" << (unsigned int) secondByte << "] " << message;
                log(debugMsg.str());
                debugMsg.clear();
            }
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
    else if (length < 65536){
        msg.push_back(126);
        msg.push_back(length >> 8);
        msg.push_back(length & 0xff);
    }
    else
        return "ERROR LENGTH";

    for (int i = 0; i < length; i++)
        msg.push_back(str[i]);

    return string(msg.begin(), msg.end());
}

int ClientConnection::hasData(int sec, int usec) {
    return sock->hasData(sec, usec);
}

void ClientConnection::run() {
    vector<string> msgs;
    isRunning = true;
    while (isConnected() && isRunning) {
        receiveMsg(msgs);

        for (vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++) {
            string rawMessage = *it;
            msgQueue.push(rawMessage);
            if (rawMessage.compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                disconnect();
            }
        }

        msgs.clear();
    }
}

void ClientConnection::stop() {
    isRunning = false;
    mainThread.join();
}

void ClientConnection::start() {
    mainThread = thread([this] {this->run();});
}
