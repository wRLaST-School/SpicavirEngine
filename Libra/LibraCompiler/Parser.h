#pragma once
#include "Lexer.h"

namespace Libra {
	class Parser
	{
	public:


	private:
		int currentToken_;

		int GetNextToken();

		Lexer lexer_;
	};
}

