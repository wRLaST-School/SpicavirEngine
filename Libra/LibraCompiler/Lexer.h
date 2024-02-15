#pragma once
#include "Token.h"
#include <string>

namespace Libra {
	class Lexer
	{
	public:
		int GetToken();

	private:

		std::string identifierStr;
		double numVal;
	};
}

