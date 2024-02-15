#include "Parser.h"

int Libra::Parser::GetNextToken()
{
	return currentToken_ = lexer_.GetToken();
}
