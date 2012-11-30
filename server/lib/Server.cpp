
#include "defs.hpp"
#include "Server.hpp"
#include "util/PracticalSocket.hpp"


using namespace std;

Server::Server(unsigned short port) {
    servSocket = new  TCPServerSocket(port);
    this->port = port;
}

void Server::stop() {
    delete servSocket;
    isRunning = false;
    mainThread.join();
}

void Server::start() {
    mainThread = thread([this] {this->run();});
}

void Server::run() {

    if (_SERVER_DEBUG) {
        stringstream debug;
        debug << "Listening at port: " << port;
        log(debug.str());
    }

    isRunning = true;
    try {
        int id = 0;

        while(isRunning == true) {
            ClientConnection * client;

            if (servSocket->hasData(0, 500)) {
                client = new ClientConnection(id++, servSocket->accept());
                clientQueue.push(client);
            }
        }

    } catch (SocketException & e) {
        if (_SERVER_ERR_DEBUG) log(e.what());
        exit(1);
    }
}

void Server::log(string msg) {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string timeStr(asctime(timeinfo));
    timeStr[timeStr.length()-1] = '\0';
    cout << timeStr.substr(11, timeStr.length()) << ":" << "Server" << ":" << msg << endl;
}

void Server::runClient(ClientConnection * client) {
    vector<string> msgs;
    while (client->isConnected()) {
        client->receiveMsg(msgs);

        for (vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++) {
            string rawMessage = *it;
            this->guidMsgQueue.push(pair<string, string>(client->guid, rawMessage));
            if (rawMessage.compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                client->disconnect();
            }
        }

        msgs.clear();
    }
}