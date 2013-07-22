/*
 * Message.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#include "Message.hpp"

Message::Message(std::string message, std::string guid, unsigned int msgId) {
	this->message = message;
	this->guid = guid;
	this->id = msgId;
}

Message::~Message() {}

