
#include "defs.hpp"
#include "ConnServer.hpp"

using namespace std;

ConnServer::ConnServer(int port) {
	clientsOnline = 0;

	server = new Server(port);
	server->start();
}

ConnServer::~ConnServer() {
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

void ConnServer::broadcast(string msg) {
	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {

		(*it).second->sendMsg(msg);
	}
}

void ConnServer::run() {
	isRunning = true;

	while (isRunning) {
		// novos clients
		while (!server->clientQueue.empty()) {
			ClientConnection * client = server->clientQueue.pop();
			clients[client->guid] = client;
			onClientConnect(client);
			client->start();

			clientsOnline++;

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

				clientsOnline--;
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

void ConnServer::stop() {
	isRunning = false;
	mainThread.join();
}

void ConnServer::start() {
	mainThread = thread([this] {this->run();});

	//stepThread = thread(step);
}
