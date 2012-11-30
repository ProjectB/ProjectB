
#include "defs.hpp"
#include "GameServer.hpp"

using namespace std;

GameServer::GameServer(int port) {
    server = new Server(port);
    server->start();
}

GameServer::~GameServer() {
    for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it).second->isConnected())
            (*it).second->disconnect();
    }
    for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
        delete ((*it).second);
        it = clients.erase(it);
    }
    server->stop();
    delete server;
}

void GameServer::broadcast(string msg) {
    for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {

        (*it).second->sendMsg(msg);
    }
}

void GameServer::run() {
    isRunning = true;

    while (isRunning) {
        // novos clients
        while (!server->clientQueue.empty()) {
            ClientConnection * client = server->clientQueue.pop();
            clients[client->guid] = client;
            onClientConnect(client);
            client->start();
        }

        // check clients
        for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
            ClientConnection * client = (*it).second;
            // disconnect
            if (!client->isConnected()) {
                client->stop();
                onClientDisconnect(client);
                delete (client);
                clients.erase((*it).first);
            }
            else {
                // novas msgs
                while (!client->msgQueue.empty()) {
                    string msg = client->msgQueue.pop();
                    onNewMessage(client->guid, msg);
                }
            }
        }

        step();
    }
}

void GameServer::stop() {
    isRunning = false;
    mainThread.join();
}

void GameServer::start() {
    mainThread = thread([this] {this->run();});
}
