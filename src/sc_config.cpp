#include "sc_config.h"

Config::Config()
{
	//Default values incase they are not included in config file
	members["WINDOW_WIDTH"] = 1280;
	members["WINDOW_HEIGHT"] = 720;
	members["FULLSCREEN"] = 0;
	members["FOV"] = 45;
}

bool Config::loadConfig(std::string filename)
{
	std::string configString(sc_file_read(filename.c_str()));

	if (configString.empty())
	{
		return false;
	}
	else
	{
		size_t delimitPos = 0;
		size_t lastPos = 0;

		std::string configPair;
		std::string configMember;
		std::string configValue;

		//Split config file into individual lines
		while ((delimitPos = configString.find("\n", lastPos)) != std::string::npos)
		{
			configPair = configString.substr(lastPos, delimitPos - lastPos);

			//Split line into member and value strings
			int wedge = configPair.find("=");
			configMember = configPair.substr(0, wedge);
			configValue = configPair.substr(wedge + 1, configPair.size());

			//Remove spaces from member and value strings
			configMember.erase(std::remove(configMember.begin(), configMember.end(), ' '), configMember.end());
			configValue.erase(std::remove(configValue.begin(), configValue.end(), ' '), configValue.end());

			//Remove Windows line endings
			configValue.erase(std::remove(configValue.begin(), configValue.end(), '\r'), configValue.end());

			//Add pair to config object
			members[configMember] = atoi(configValue.c_str());

			lastPos = delimitPos + 1;
		}
	}

	return true;
}

int Config::get(std::string member)
{
	return members[member];
}