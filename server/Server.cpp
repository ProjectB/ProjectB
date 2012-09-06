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
            while (client->isConnected()) {

                // server de eco temporario
                    string message = client->receiveMsg();
                    cout << "Client-message: " << message << endl;
                    client->sendMsg("ok\r\n\r\n");
                    if (message.compare("_0x8_connection_close") == 0) {
                        cout << "connection closed!" << endl;
                        break;
                    } else if (message.compare("_0x9_ping") == 0) {
                    } else if (message.compare("_0xA_pong") == 0) {
                    } else {
                    }

            }
            delete client;
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
