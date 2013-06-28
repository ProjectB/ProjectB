
#include "../defs.hpp"
#include "MsgServer.hpp"

using namespace std;

MsgServer::MsgServer(unsigned int port) {
	isRunning = false;
	messagesProccessed=0;
	hasDataSec = 0;
	hasDataUSec = 500;

	server = new Server(port);
	start();
}

MsgServer::~MsgServer() {
	isRunning = false;
	server->~server();
	delete server;
	mainThread.join();
}

void MsgServer::start() {
	isRunning = true;
	sendThread = thread([this] {this->processQueue();});
	recvThread = thread([this] {this->receiveMessages();});

	//http://en.cppreference.com/w/cpp/chrono/c/time
	std::time_t result = std::time(NULL);
	serverStartTime = std::asctime(std::localtime(&result));

	Logger::log("Message Server starting @" + serverStartTime);
}

void MsgServer::broadcast(string msg) {
	/*
	for (map<string, ClientConnection *>::iterator it = clients.begin(); it != clients.end(); it++) {

		(*it).second->sendMsg(msg);
	}
	*/
}

void MsgServer::processQueue() {

	while(isRunning) {
	}
}

void MsgServer::receiveMessages() {

	while(isRunning) {
	}
}





