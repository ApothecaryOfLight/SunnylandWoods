#include "logger.hpp"
#include <fstream>
#include <string>
#include <sstream>

Logger::Logger(void) {
	outStream.open("log.txt", std::fstream::out);
}

Logger::~Logger(void) {
	outStream.close();
}

void Logger::log(std::string inMessage) {
	outStream << inMessage << std::endl;
}
void Logger::log(int inInteger) {
	std::stringstream myStringstreamConv;
	myStringstreamConv << inInteger;
	std::string myConv = myStringstreamConv.str();
	outStream << myConv << std::endl;
}
void Logger::log(float inFloat) {
	std::stringstream myStringstreamConv;
	myStringstreamConv.precision(5);
	myStringstreamConv << inFloat;
	std::string myConv = myStringstreamConv.str();
	outStream << myConv << std::endl;
}
