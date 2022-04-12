#pragma once

enum class Cond2 {
	FF,
	TF,
	FT,
	TT,
};

/*
処理効率が良くないため非推奨。
二個のboolの組み合わせを返す。
用例：
switch(DoubleCond( isA, isB ))
{
	case Cond2::FF:
		break;
	
	case Cond2::TF:
		break;

	case Cond2::FT:
		break;

	case Cond2::TT:
		break;
}
*/
Cond2 DoubleCond(bool first, bool second);

