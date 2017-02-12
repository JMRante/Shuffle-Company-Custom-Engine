/*

    SHUFFLE COMPANY SOURCE CODE

    sc_tokenizer.h
    ------------------------------------



*/

#ifndef SC_TOKENIZER
#define SC_TOKENIZER

#include <vector>
#include <utility>
#include <string>
#include <cctype>

#include "sc_log.h"

namespace sc
{
	class Tokenizer
	{
	private:
		std::string currentType;
		std::string currentToken;
		std::vector<char> delimiters;
		std::istream* stream;

	public:
		Tokenizer(std::istream* stream);
		bool next();
		void addDelimiter(char delimiter);
		std::string getType();
		std::string getToken();
		bool checkType(std::string type);
		bool checkToken(std::string token);
		bool check(std::string type, std::string token);
	};
}

#endif