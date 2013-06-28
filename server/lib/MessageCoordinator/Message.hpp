/*
 * Message.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kodo
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

class Message {
private:
	std::string message;
	std::string senderIp;
	std::string senderName;
	std::string destinationIp;
	unsigned long id;
	bool delivered;

public:
	/* getters and setters */
	const std::string& getDestinationIp() const { return destinationIp; }
	unsigned long getId() const { return id; }
	const std::string& getMessage() const { return message; }
	const std::string& getSenderIp() const { 	return senderIp; }
	const std::string& getSenderName() const { return senderName; }

	const bool& getDelivered() const { return delivered; }
	void setDelivered(const bool& delivered) { this->delivered = delivered; }

	Message(std::string message, std::string senderIp, std::string senderName, std::string dest, unsigned long msgId);
	~Message();
};


#endif /* MESSAGE_HPP_ */
