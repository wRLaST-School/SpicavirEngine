#include "Lexer.h"
#include "Identifier.h"
#include "CodeObject.h"

int Libra::Lexer::GetToken()
{
	static int lastChar = ' ';

	while (isspace(lastChar))
	{
		lastChar = CodeObject::GetNextChar();
	}

	//なんらかの単語である場合
	if (isalpha(lastChar))
	{
		identifierStr = (char)lastChar;
		while (isalnum((lastChar = CodeObject::GetNextChar())))
		{
			identifierStr += (char)lastChar;
		}

		//IdentifierTable(予約語一覧)から一致するものがあればそのトークンを返す
		for (auto& id : identifierTable)
		{
			if (identifierStr == id.first)
			{
				return id.second;
			}
		}

		//どのIdentifierとも一致しなかったらtok_identifierを返す
		return tok_identifier;
	}

	//なんらかの数値である場合
	if (isdigit(lastChar) || lastChar == '.') {
		std::string numStr;
		do {
			numStr += (char)lastChar;
			lastChar = CodeObject::GetNextChar();
		} while (isdigit(lastChar) || lastChar == '.');

		numVal = strtod(numStr.c_str(), 0);

		return tok_number;
	}

	//コメントである場合
	if (lastChar == '#') {
		//行の終わりまでコメント
		do {
			lastChar = CodeObject::GetNextChar();
		} while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

		if (lastChar != EOF)
		{
			//コメント行が終わったらそのまま次のトークンを見に行く
			return GetToken();
		}
	}

	if (lastChar == EOF)
		return tok_eof;

	//何にも当てはまならいなら文字のASCIIコードをそのまま返却
	int thisChar = lastChar;
	lastChar = CodeObject::GetNextChar();
	return thisChar;
}
