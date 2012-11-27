
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

            thread t = thread([this, client] {this->runClient(client);});
            t.detach();

            onClientConnect(client);
        }

        // novas msgs
        while (!guidMsgQueue.empty()) {
            pair<string, string> guidMsg = guidMsgQueue.pop();
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

void GameServer::runClient(ClientConnection * client) {
    vector<string> msgs;
    while (client->isConnected()) {
        client->receiveMsg(msgs);

        for (vector<string>::iterator it = msgs.begin(); it != msgs.end(); it++) {
            string rawMessage = *it;
            this->guidMsgQueue.push(pair<string, string>(client->guid, rawMessage));
            if (rawMessage.compare(0, strlen("_0x8_connection_close"), "_0x8_connection_close") == 0) {
                client->disconnect();
            }
        }

        msgs.clear();
    }
}

