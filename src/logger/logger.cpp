#include "logger.hpp"
#include <fstream>
#include <string>
#include <sstream>

Logger::Logger(void) {
	outStream.open("log.txt", std::fstream::out | std::fstream::app);
}

Logger::~Logger(void) {
	outStream.close();
}

void Logger::log(std::string inMessage) {
	//std::fstream outStream;
	//outStream.open("log.txt", std::fstream::out | std::fstream::app);
	outStream << inMessage << std::endl;
}
void Logger::log(int inInteger) {
	std::stringstream myStringstreamConv;
	myStringstreamConv << inInteger;
	std::string myConv = myStringstreamConv.str();
	//std::fstream outStream;
	//outStream.open("log.txt", std::fstream::out | std::fstream::app);
	outStream << myConv << std::endl;
}
