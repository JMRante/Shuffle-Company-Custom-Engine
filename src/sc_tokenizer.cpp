/*

    SHUFFLE COMPANY SOURCE CODE

    sc_tokenizer.cpp
    ------------------------------------



*/

#include "sc_tokenizer.h"

namespace sc
{
	Tokenizer::Tokenizer(std::istream* stream)
	{
		this->stream = stream;
	}

	bool Tokenizer::next()
	{
		char c = stream->peek();
		currentToken.clear();
		currentType = Token::none;

		if (c != EOF)
		{
			while (stream->peek() == '\r' || stream->peek() == '\n' || stream->peek() == ' ')
			{
				stream->get(c);
			}

			c = stream->peek();

			if (std::isdigit(c))
			{
				currentType = Token::integer;

				while (std::isdigit(stream->peek()))
				{
					stream->get(c);
					currentToken += c;
				}

				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == ';')
			{
				currentType = Token::semicolon;
				stream->get(c);
				currentToken += c;
				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == ':')
			{
				currentType = Token::colon;
				stream->get(c);
				currentToken += c;
				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == ',')
			{
				currentType = Token::comma;
				stream->get(c);
				currentToken += c;
				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == '(')
			{
				currentType = Token::paranl;
				stream->get(c);
				currentToken += c;
				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == ')')
			{
				currentType = Token::paranr;
				stream->get(c);
				currentToken += c;
				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else if (c == '"')
			{
				currentType = Token::string;
				stream->get(c);

				while (stream->peek() != '"' && stream->peek() != EOF)
				{
					stream->get(c);
					currentToken += c;
				}

				if (stream->peek() == '"')
				{
					stream->get(c);
					return true;
				}

				if (stream->peek() == EOF)
				{
					LOG_E << "Bad token, incomplete string";
					return true;
				}

				return false;
			}
			else if (std::isalpha(c))
			{
				currentType = Token::word;

				while (std::isalnum(stream->peek()) || stream->peek() == '_')
				{
					stream->get(c);
					currentToken += c;
				}

				LOG_D << "CurrentToken: " << currentToken; LOG_FLUSH;
				return true;
			}
			else
			{
				LOG_E << "Bad token read";
				currentType = Token::none;
				currentToken.clear();
				return false;
			}	
		}

		currentType = Token::eof;
		currentToken = "eof";
		return false;
	}

	Token Tokenizer::getType()
	{
		return currentType;
	}

	std::string Tokenizer::getToken()
	{
		return currentToken;
	}

	bool Tokenizer::check(Token type)
	{
		if (type == currentType)
		{
			return true;
		}

		return false;
	}

	bool Tokenizer::check(std::string token)
	{
		if (token.compare(currentToken) == 0)
		{
			return true;
		}

		return false;
	}

	bool Tokenizer::check(Token type, std::string token)
	{
		if (type == currentType && token.compare(currentToken) == 0)
		{
			return true;
		}

		return false;		
	}
}