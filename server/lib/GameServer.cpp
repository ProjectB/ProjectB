
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

            thread t = thread([this, client] {server->runClient(client);});
            t.detach();

            onClientConnect(client);
        }

        // novas msgs
        while (!server->guidMsgQueue.empty()) {
            pair<string, string> guidMsg = server->guidMsgQueue.pop();
            onNewMessage(guidMsg.first, guidMsg.second);
        }

        // check clients
        for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
            if (!(*it).second->isConnected()) {
                onClientDisconnect((*it).second);
                delete ((*it).second);
                clients.erase((*it).first);
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
