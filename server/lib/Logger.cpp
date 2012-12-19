/*
 * Logger.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: ricardo
 */

#include "Logger.hpp"

using namespace std;

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::log(string msg) {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string timeStr(asctime(timeinfo));
    timeStr[timeStr.length()-1] = '\0';
    cout << timeStr.substr(11, timeStr.length()) << ":" << "Server" << ":" << msg << endl;
}
