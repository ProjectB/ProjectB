#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <cstring>

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

            cout << "SERVER: new client " << client->str() << endl;

            thread t = thread([this, client] {this->runClient(client);});
            t.detach();
        }

        // atualiza estados
        while (!msgQueue.empty())
            msgs.push_back(msgQueue.pop());

        // broadcast
        for (vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end();) {
            if (!(*it)->isConnected()) {
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
    time_t rawtime;
    struct tm * timeinfo;

    while (client->isConnected()) {
        string rawMessage = client->receiveMsg();

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        string timeStr(asctime(timeinfo));
        timeStr.erase(timeStr.length() - 1, timeStr.length() - 1);

        stringstream ss;
        ss << timeStr << ":Client(" << client->str() << "):" << rawMessage;
        string message = ss.str();

        cout << message << endl;
        this->msgQueue.push(message);

        if (rawMessage.compare(0, strlen("_0x8_connection_close") - 1, "_0x8_connection_close") == 0) {
            client->disconnect();
        } else if (rawMessage.compare(0, strlen("_0x9_ping") - 1, "_0xA_ping") == 0) {
            client->disconnect();
        } else if (rawMessage.compare(0, strlen("_0xA_pong") - 1, "_0xA_pong") == 0) {
            client->disconnect();
        }
    }
}

