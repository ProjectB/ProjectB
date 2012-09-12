#include <iostream>
#include <cstdlib>
#include <thread>
#include "ClientConnection.hpp"
#include "GameServer.hpp"
#include "Server.hpp"
#include "lib/PracticalSocket.hpp"

using namespace std;

Server::Server(unsigned short port) :
        isRunning() {
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
    isRunning = true;
    cout << "Listening at port: " << port << endl;
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
        cerr << e.what() << endl;
        exit(1);
    }

    gs->stop();
}
