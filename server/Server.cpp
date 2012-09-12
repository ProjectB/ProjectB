#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>
#include "ClientConnection.hpp"
#include "GameServer.hpp"
#include "Server.hpp"
#include "lib/PracticalSocket.hpp"
#include "defs.hpp"

using namespace std;

Server::Server(unsigned short port) : isRunning() {
    gs = new GameServer();
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
        gs->start();
        int id = 0;

        while(isRunning == true) {
            ClientConnection * client;

            if (servSocket->hasData(0, 500)) {
                client = new ClientConnection(id++, servSocket->accept());
                gs->clientQueue.push(client);
            }
        }

    } catch (SocketException & e) {
        if (_SERVER_ERR_DEBUG) log(e.what());
        exit(1);
    }

    gs->stop();
    delete gs;
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
