/*

	SHUFFLE COMPANY SOURCE CODE

	sc_log.h
	------------------------------------

	The Log class allows for debug and error output to a file for examination after runtime.

*/


#ifndef SC_LOG
#define SC_LOG

#include <iostream>
#include <fstream>
#include <string>

#include "sc_config.h"

namespace sc
{
	extern std::ofstream logStream;

	void openLog(std::string logFile);
	void closeLog();
}

#define LOG_E sc::logStream << "\nERROR: "
#define LOG_D if (!sc::config.get("LOG_DEBUG")) {} else sc::logStream << "\nDEBUG: "
#define LOG_I if (!sc::config.get("LOG_INFO")) {} else sc::logStream << "\nINFO: "
#define LOG_EC sc::logStream
#define LOG_DC if (!sc::config.get("LOG_DEBUG")) {} else sc::logStream
#define LOG_IC if (!sc::config.get("LOG_INFO")) {} else sc::logStream
#define LOG_FLUSH sc::logStream.flush()


#endif