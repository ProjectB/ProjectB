#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "lib/PracticalSocket.hpp"
#include "ClientConnection.hpp"

ClientConnection::ClientConnection(TCPSocket* sock) {
    const unsigned int RECVBUFSIZE = 32;

    cout << "Handling client ";
    try {
        cout << sock->getForeignAddress() << ":";
    } catch (const SocketException& e) {
        cerr << "Unable to get foreign address" << endl;
    }
    try {
        cout << sock->getForeignPort();
    } catch (const SocketException& e) {
        cerr << "Unable to get foreign port" << endl;
    }
    cout << endl;

    char recvBuffer[RECVBUFSIZE];
    unsigned int recvMsgSize;
    stringstream msg;

    while ((recvMsgSize = sock->recv(recvBuffer, RECVBUFSIZE)) > 0) {
        int pos;
        if ((pos = string(recvBuffer).find("\r\n\r\n")) != string::npos) {
            recvBuffer[pos] = 0;
            msg << recvBuffer;
            break;
        }
        msg << recvBuffer;
    }

    answerWSClient(sock, msg.str());
}

void ClientConnection::answerWSClient(TCPSocket* sock, string msg) {
    string key;
    vector<string> lines;
    istringstream iss(msg);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string>>(lines));

    for (vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
        cout << *i << endl;
    }
}

