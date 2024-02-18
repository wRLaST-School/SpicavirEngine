#pragma once
#include "Lexer.h"
#include "ExprAST.h"
#include <map>
#include <memory>

namespace Libra {
	class Parser
	{
	public:
		void ParserLog(std::string str);

		void ParseCode();

		void HandleDefinition();
		void HandleExtern();
		void HandleTopLevelExpression();

		std::unique_ptr<ExprAST> ParseNumberExpr();
		std::unique_ptr<ExprAST> ParseParenExpr();
		std::unique_ptr<ExprAST> ParseIdentifierExpr();
		std::unique_ptr<ExprAST> ParsePrimary();
		std::unique_ptr<ExprAST> ParseExpression();
		std::unique_ptr<ExprAST> ParseBinOpRHS(int exprPrec, std::unique_ptr<ExprAST> lhs);
		std::unique_ptr<PrototypeAST> ParsePrototype();
		std::unique_ptr<FunctionAST> ParseDefinition();
		std::unique_ptr<PrototypeAST> ParseExtern();
		std::unique_ptr<FunctionAST> ParseTopLevelExpr();
	private:
		int currentToken_;

		int GetNextToken();

		int GetTokenPrecedence();
		Lexer lexer_;

		std::map<char, int> binaryOperatorPrecedence{
			{'<', 10},
			{'>', 10},
			{'+', 20},
			{'-', 20},
			{'*', 30},
			{'/', 30}
		};
	};
}

