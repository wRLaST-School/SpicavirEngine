#include "Parser.h"
#include <Windows.h>

using namespace Libra;

void Libra::Parser::ParserLog(std::string str)
{
	OutputDebugStringA((std::string("Parser: ") + str + std::string("\n")).c_str());
}

void Libra::Parser::ParseCodeLoop()
{
	ParserLog("Starting Parser...");
	while (true) {
		switch (currentToken_) {
		case tok_eof: return;
		case ';': GetNextToken(); break; // セミコロンを無視する
		case tok_def: HandleDefinition(); break;
		case tok_extern: HandleExtern(); break;
		default: HandleTopLevelExpression(); break;
		}

	}
}

void Libra::Parser::HandleDefinition()
{
	if (auto FnAST = ParseDefinition()) {
		FnAST->CodeGen();
		ParserLog("Parsed a func definition");
	}
	else {
		//エラー対処の為一つトークンを飛ばす
		GetNextToken();
	}
}

void Libra::Parser::HandleExtern()
{
	if (auto FnAST = ParseExtern()) {
		FnAST->CodeGen();
		ParserLog("Parsed an extern");
	}
	else {
		//エラー対処の為一つトークンを飛ばす
		GetNextToken();
	}
}

void Libra::Parser::HandleTopLevelExpression()
{
	if (auto FnAST = ParseTopLevelExpr()) {
		FnAST->CodeGen();
		ParserLog("Parsed a top-level expression");
	}
	else {
		//エラー対処の為一つトークンを飛ばす
		GetNextToken();
	}
}

std::unique_ptr<ExprAST> Libra::Parser::ParseNumberExpr()
{
	std::unique_ptr<ExprAST> result = std::make_unique<NumberExprAST>(lexer_.numVal);

	GetNextToken();

	return result;
}

std::unique_ptr<ExprAST> Libra::Parser::ParseParenExpr()
{
	GetNextToken();
	std::unique_ptr<ExprAST> v = ParseExpression();

	if (!v) return nullptr;

	if (currentToken_ != ')')
	{
		return Error("expected ')'");
	}

	GetNextToken(); // ")"を消費する

	return v;
}

std::unique_ptr<ExprAST> Libra::Parser::ParseIdentifierExpr()
{
	std::string idName = lexer_.identifierStr;

	GetNextToken(); // 識別子を消費

	if (currentToken_ != '(') // 次が'('でないなら変数を参照しているだけ
	{
		return std::make_unique<VariableExprAST>(idName);
	}

	// '('だったので関数呼び出し
	GetNextToken(); // '('だったので'('を消費

	std::vector<std::unique_ptr<ExprAST>> args;

	if (currentToken_ != ')') {
		while (true) {
			std::unique_ptr<ExprAST> arg = ParseExpression();
			if (!arg) return 0;
			args.push_back(std::move(arg));

			if (currentToken_ == ')') break;

			if (currentToken_ != ',')
			{
				return Error("Expected ')' or ',' in argument list");
			}

			GetNextToken();
		}
	}

	GetNextToken(); // ')'を消費

	return std::make_unique<CallExprAST>(idName, std::move(args));
}

std::unique_ptr<ExprAST> Libra::Parser::ParsePrimary()
{
	switch (currentToken_) {
	case tok_identifier: return ParseIdentifierExpr();
	case tok_number: return ParseNumberExpr();
	case '(': return ParseParenExpr();

	default: return Error("unknown token when expecting an expression");
	}
}

std::unique_ptr<ExprAST> Libra::Parser::ParseExpression()
{
	std::unique_ptr<ExprAST> lhs = ParsePrimary();
	if (!lhs) return nullptr;

	return ParseBinOpRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAST> Libra::Parser::ParseBinOpRHS(int exprPrec, std::unique_ptr<ExprAST> lhs)
{
	while (true) {
		int tokenPrec = GetTokenPrecedence();

		// 演算子の優先順位がExprPrecより大きければ処理し、
		// そうでなければ終了する
		if (tokenPrec < exprPrec)
		{
			return lhs;
		}

		int binOp = currentToken_;

		GetNextToken();

		// 二項演算子の後のプライマリ式を解析する
		std::unique_ptr<ExprAST> rhs = ParsePrimary();

		if (!rhs) return nullptr;

		//binOpの優先順位がrhsの後の演算子より低いなら、処理中の演算子はrhsをそのlhsとして受け取る
		int nextPrec = GetTokenPrecedence();

		if (tokenPrec < nextPrec)
		{
			rhs = ParseBinOpRHS(tokenPrec + 1, std::move(rhs));

			if (!rhs) return nullptr;
		}

		//lhsとrhsをマージして一つのブロックにする
		lhs = std::make_unique<BinaryExprAST>((char)binOp, std::move(lhs), std::move(rhs));
	}
}

std::unique_ptr<PrototypeAST> Libra::Parser::ParsePrototype()
{
	if (currentToken_ != tok_identifier)
		return ErrorP("Expected function name in prototype");

	std::string fnName = lexer_.identifierStr;
	GetNextToken();

	if (currentToken_ != '(')
		return ErrorP("Expected '(' in prototype");

	// 引数の名前のリストを読み取る
	std::vector<std::string> argNames;
	while (GetNextToken() == tok_identifier)
		argNames.push_back(lexer_.identifierStr);
	if (currentToken_ != ')')
		return ErrorP("Expected ')' in prototype");

	// 成功
	GetNextToken();  // ')'を消費。

	return std::make_unique<PrototypeAST>(fnName, argNames);
}

std::unique_ptr<FunctionAST> Libra::Parser::ParseDefinition()
{
	GetNextToken(); // 関数定義キーワードを消費(現在は"def"のみ)

	std::unique_ptr <PrototypeAST> proto = ParsePrototype();
	if (!proto) return nullptr;

	if (std::unique_ptr<ExprAST> e = ParseExpression())
	{
		return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
	}

	return nullptr;
}

std::unique_ptr<PrototypeAST> Libra::Parser::ParseExtern()
{
	GetNextToken(); // "extern"を消費
	return ParsePrototype();
}

std::unique_ptr<FunctionAST> Libra::Parser::ParseTopLevelExpr()
{
	if (std::unique_ptr<ExprAST> e = ParseExpression()) {
		std::unique_ptr<PrototypeAST> proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
		return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
	}

	return nullptr;
}

int Libra::Parser::GetNextToken()
{
	return currentToken_ = lexer_.GetToken();
}

int Libra::Parser::GetTokenPrecedence()
{
	if (!isascii(currentToken_)) return -1;

	int tokenPrec = binaryOperatorPrecedence[(char)currentToken_];
	//未定義の演算子なら-1を返す
	if (tokenPrec <= 0) return -1;

	return tokenPrec;
}
