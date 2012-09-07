#include <iostream>
#include <cstdlib>
#include <thread>
#include "ClientConnection.hpp"
#include "GameServer.hpp"
#include "Server.hpp"
#include "lib/PracticalSocket.hpp"

using namespace std;

Server::Server(unsigned short port) {
    this->port = port;
}

void Server::run() {
    cout << "Listening at port: " << port << endl;
    try {
        GameServer * gs = new GameServer();
        TCPServerSocket servSocket(port);

        gs->start();

        for (int id = 0;; id++) {
            ClientConnection * client = new ClientConnection(id, servSocket.accept());
            gs->clientQueue.push(client);

        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
