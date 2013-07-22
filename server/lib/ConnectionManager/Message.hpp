/*
 * Message.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>

class Message {
private:
	std::string message;
	std::string guid;
	unsigned int id;

public:
	/* getters and setters */
	unsigned int getId() const { return id; }
	const std::string& getMessage() const { return message; }
	const std::string& getGuid() const { return guid; }

	Message(std::string message, std::string guid, unsigned int msgId);
	~Message();
};


#endif /* MESSAGE_HPP_ */
