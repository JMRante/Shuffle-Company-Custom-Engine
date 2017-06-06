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
	enum Token {none, integer, semicolon, colon, comma, paranl, paranr, string, word, eof};
	class Tokenizer
	{
	private:
		Token currentType;
		std::string currentToken;
		std::istream* stream;

	public:
		Tokenizer(std::istream* stream);
		bool next();
		Token getType();
		std::string getToken();
		bool check(Token type);
		bool check(std::string token);
		bool check(Token type, std::string token);
	};
}

#endif