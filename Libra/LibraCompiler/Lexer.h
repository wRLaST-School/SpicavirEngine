#pragma once
#include "Token.h"
#include <string>

namespace Libra {
	class Lexer
	{
	public:
		void Lex();

	private:
		int GetToken();

		std::string identifierStr;
		double numVal;
	};
}

