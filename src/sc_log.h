#ifndef SC_LOG
#define SC_LOG

#include <string>
#include <iostream>
#include <fstream>

class Log
{
private:
	static std::string logBuffer;
	std::string logName;
public:
	Log(std::string logName);
	void openLogFile(std::string filename);
	void log(std::string logMessage);
	void closeLogFile();
}

#endif