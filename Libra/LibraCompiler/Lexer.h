#pragma once
#include "Token.h"
#include <string>

namespace Libra {
	class Parser;

	class Lexer
	{
	public:
		int GetToken();

	private:
		friend Parser;

		std::string identifierStr;
		double numVal;
	};
}

