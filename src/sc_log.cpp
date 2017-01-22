/*

	SHUFFLE COMPANY SOURCE CODE

	sc_log.cpp
	------------------------------------

	The Log class allows for debug and error output to a file for examination after runtime.

*/

#include "sc_log.h"

namespace sc
{
	std::ofstream logStream;

	void openLog(std::string logFile)
	{
		sc::logStream.open(logFile, std::ofstream::out | std::ofstream::trunc);
		sc::logStream << "Shuffle Company Log\n------------------------";
	}

	void closeLog()
	{
		sc::logStream.close();
	}
}