#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <thread>

#include "GameServer.hpp"
#include "MultithreadQueue.hpp"

using namespace std;

GameServer::GameServer() {

}

void GameServer::run() {
    while(true) {
        // atuliza clientes
        while (!clientQueue.empty()) {
            ClientConnection * client = clientQueue.pop();
            clients.push_back(client);
            //std::thread t(this->runClient, client);
            //t.detach();
        }

        // atualiza estados
        while (!msgQueue.empty())
            msgs.push_back(msgQueue.pop());

        // broadcast
        for(vector<ClientConnection *>::iterator it = clients.begin(); it != clients.end(); )
        {
            if ((*it)->isConnected()) {
                it = clients.erase(it);
                continue;
            }

            for(vector<string>::iterator jt = msgs.begin(); jt != msgs.end(); jt++)
                (*it)->sendMsg(*jt);
            it++;
        }

        msgs.clear();
    }
}

void GameServer::start() {
    //thread t1(run, this);
    //Description   Resource    Path    Location    Type
}

void GameServer::runClient(ClientConnection * client) {
    while (client->isConnected()) {
        // server de eco temporario
        string message = client->receiveMsg();
        cout << "Client(" << client->address << ":" << client->port << "): " << message << endl;
        msgQueue.push(message);
        if (message.compare("_0x8_connection_close") == 0) {
            client->connMutex.lock();
            break;
        } else if (message.compare("_0x9_ping") == 0) {
        } else if (message.compare("_0xA_pong") == 0) {
        } else {
        }
    }
    client->disconnect();
}

