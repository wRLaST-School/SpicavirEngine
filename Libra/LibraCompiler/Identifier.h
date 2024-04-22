#pragma once
#ifndef LIBRA_IDENTIFIER_DEF_
#define LIBRA_IDENTIFIER_DEF_

#include <unordered_map>
#include "Token.h"
#include <string>

namespace Libra {
	std::unordered_map<std::string, Token> identifierTable = {
		{"def", tok_def},
		{"extern", tok_extern},
		{"if", tok_if},
		{"then", tok_then},
		{"else", tok_else}
	};
}

#endif