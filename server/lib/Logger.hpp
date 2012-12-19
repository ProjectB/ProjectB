/*
 * Logger.hpp
 *
 *  Created on: Dec 19, 2012
 *      Author: ricardo
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <string>

class Logger {
public:
    Logger();
    virtual ~Logger();
    void log(std::string msg);
};

#endif /* LOGGER_HPP_ */
