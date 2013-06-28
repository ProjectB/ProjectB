/*
 * Message.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */


Message::Message(std::string message, std::string senderIp, std::string senderName, std::string dest, unsigned long msgId) {
	this->message = message;
	this->senderIp = senderIp;
	this->senderName = senderName;
	this->destinationIp = dest;
	this->id = msgId;
	this->delivered = false;
}

Message::~Message() {}

