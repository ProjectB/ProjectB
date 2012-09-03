#include <iostream>
#include <cstdlib>
#include "ClientConnection.hpp"

#include "Server.hpp"
#include "lib/PracticalSocket.hpp"

using namespace std;

Server::Server(unsigned short port) {
    this->port = port;
}

void Server::start() {
    cout << "Listening at port: " << port << endl;
    try {
        TCPServerSocket servSocket(port);
        for (;;) {
            ClientConnection * client = new ClientConnection(servSocket.accept());
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
