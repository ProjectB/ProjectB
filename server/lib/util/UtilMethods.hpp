/*
 * UtilMethods.hpp
 *
 *  Created on: Feb 15, 2015
 *      Author: kodo
 */

#ifndef UTILMETHODS_HPP_
#define UTILMETHODS_HPP_

#include <string>
#include <vector>

namespace UtilMethods
{
	std::vector<std::string> splitInputMessage(std::string);
	std::string getMessageTopic(std::string);
	std::vector<std::string> getMessageAttributes(unsigned int, std::string);
}


#endif /* UTILMETHODS_HPP_ */
