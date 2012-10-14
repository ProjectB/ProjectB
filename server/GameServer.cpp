#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring>

#include "defs.hpp"
#include "GameServer.hpp"
//#include "MultithreadQueue.hpp"

using namespace std;

GameServer::GameServer() {

}

GameServer::~GameServer() {
    for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->isConnected())
            (*it)->disconnect();
    }
    for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
        delete(*it);
        it = clients.erase(it);
    }
}

void GameServer::broadcast(string msg) {
    for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {
        (*it)->sendMsg(msg);
    }
}

void GameServer::run() {
    isRunning = true;

    while (isRunning) {
        // novos clients
        while (!clientQueue.empty()) {
            ClientConnection * client = clientQueue.pop();
            clients.push_back(client);

            thread t = thread([this, client] {this->runClient(client);});
            t.detach();

            onClientConnect(client);
        }

        // novas msgs
        while (!msgQueue.empty()) {
            string msg = msgQueue.pop();
            msgs.push_back(msg);
            onNewMessage(msg);
        }

        // check clients
        for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
            if (!(*it)->isConnected()) {
                onClientDisconnect(*it);
                delete(*it);
                it = clients.erase(it);
                continue;
            }
            it++;
        }

        step();

        .clear();
    }
}

void GameServer::stop() {
    isRunning = false;
    mainThread.join();
}

void GameServer::start() {
    mainThread = thread([this] {this->run();});
}

void GameServer::runClient(ClientConnection * client) {
    vector<string> msgs;
    while (client->isConnected()) {
        client->receiveMsg(msgs);

        for (vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++) {
            string rawMessage = *it;
            this->msgQueue.push(rawMessage);
            if (rawMessage.compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                client->disconnect();
            }
        }

        msgs.clear();
    }
}

