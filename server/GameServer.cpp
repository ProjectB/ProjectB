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

GameServer::GameServer() :
        isRunning() {
}

void GameServer::run() {
    isRunning = true;

    while (isRunning) {
        // atuliza clientes
        while (!clientQueue.empty()) {
            ClientConnection * client = clientQueue.pop();
            clients.push_back(client);

            thread t = thread([this, client] {this->runClient(client);});
            t.detach();
        }

        // atualiza estados
        while (!msgQueue.empty())
            msgs.push_back(msgQueue.pop());

        // broadcast
        for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
            if (!(*it)->isConnected()) {
                delete(*it);
                it = clients.erase(it);
                continue;
            }

            for (vector<string>::iterator jt = msgs.begin(); jt != msgs.end(); jt++)
                (*it)->sendMsg(*jt);
            it++;
        }
        msgs.clear();
    }

    for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
        if ((*it)->isConnected())
            (*it)->disconnect();
        it = clients.erase(it);
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

