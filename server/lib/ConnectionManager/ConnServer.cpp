#include "../defs.hpp"
#include "ConnServer.hpp"
#include "../util/PracticalSocket.hpp"
#include "../Logger.hpp"

using namespace std;

ConnServer::ConnServer(unsigned short port) {
    servSocket = new  TCPServerSocket(port);
    this->port = port;
}

void ConnServer::stop() {
    delete servSocket;
    isRunning = false;
    mainThread.join();
}

void ConnServer::start() {
    mainThread = thread([this] {this->run();});
}

void ConnServer::run() {

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
