#include <iostream>
#include <cstdlib>
#include <thread>
#include "ClientConnection.hpp"
#include "Server.hpp"
#include "lib/PracticalSocket.hpp"

using namespace std;

Server::Server(unsigned short port) {
    this->port = port;
}

void tempFun(ClientConnection * client) {
    while (client->isConnected()) {

        // server de eco temporario
        string message = client->receiveMsg();
        cout << "Client(" << client->address << ":" << client->port << "): " << message << endl;
        client->sendMsg(message);
        if (message.compare("_0x8_connection_close") == 0) {
            //cout << "connection closed!" << endl;
            break;
        } else if (message.compare("_0x9_ping") == 0) {
        } else if (message.compare("_0xA_pong") == 0) {
        } else {
        }
    }
    delete client;
}

void Server::start() {
    cout << "Listening at port: " << port << endl;
    try {
        TCPServerSocket servSocket(port);
        for (int id = 0;; id++) {
            ClientConnection * client = new ClientConnection(id, servSocket.accept());
            thread t(tempFun, client);
            t.detach(); // precisa se n for dar join
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
