#ifndef _LOGGER_
#define _LOGGER_

#include <fstream>
#include <string>
#include <sstream>

class Logger {
public:
	Logger(void);
	~Logger(void);
	void log(std::string inMessage);
	void log(int inInteger);
};

#endif _LOGGER_