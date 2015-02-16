/*
 * UtilMethods.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: kodo
 */

#include "UtilMethods.hpp"
#include "../defs.hpp"

namespace UtilMethods
{
	std::vector<std::string> splitInputMessage(std::string msg)
	{
		std::vector<std::string> output;

		std::string currMsg;
		for(unsigned int i=0; i < msg.length(); i++)
		{
			if(msg[i] == (char)SEPARATOR)
			{
				output.push_back(currMsg);
				currMsg.clear();
			}
			else
			{
				currMsg.push_back(msg[i]);
			}
		}
		return output;
	}

	std::string getMessageTopic(std::string msg)
	{
		std::string topic;

		unsigned int i=0;
		unsigned int len = msg.length();

		while(msg[i] != '|' && msg[i] != ':' && i < len)
			topic.push_back(msg[i++]);

		return topic;
	}

	std::vector<std::string> getMessageAttributes(unsigned int offset, std::string msg)
	{
		std::vector<std::string> attributes;
		unsigned int i=offset;
		std::string str = "";

		while(msg[i++] != ':');

		for(;i < msg.size(); i++)
		{
			while(msg[i] != ';' && i < msg.size())
				str.push_back(msg[i++]);
			attributes.push_back(str);
			str.clear();
		}

		return attributes;
	}
}

