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
		bool gotDelimiters = false;

		if (c != EOF)
		{
			while (c != EOF)
			{
				if (std::find(delimiters.begin(), delimiters.end(), c) == delimiters.end())
				{
					if (gotDelimiters == true)
					{
						return true;
					}

					if (std::isdigit(c) && currentToken.size() == 0)
					{
						currentType = "int";
					}
					else if (c == ';')
					{
						currentType = "statementEnd";
					}
					else
					{
						currentType = "word";
					}

					stream->get(c);
					currentToken += c;
				}
				else
				{
					gotDelimiters = true;
					stream->get(c);
				}

				c = stream->peek();
			}

			return false;	
		}

		currentToken = "null";
		currentType = "null";
		return false;
	}

	void Tokenizer::addDelimiter(char delimiter)
	{
		delimiters.push_back(delimiter);
	}

	std::string Tokenizer::getType()
	{
		return currentType;
	}

	std::string Tokenizer::getToken()
	{
		return currentToken;
	}

	bool Tokenizer::checkType(std::string type)
	{
		if (type.compare(currentType) == 0)
		{
			return true;
		}

		return false;
	}

	bool Tokenizer::checkToken(std::string token)
	{
		if (token.compare(currentToken) == 0)
		{
			return true;
		}

		return false;
	}

	bool Tokenizer::check(std::string type, std::string token)
	{
		if (type.compare(currentType) == 0 && token.compare(currentToken) == 0)
		{
			return true;
		}

		return false;		
	}
}