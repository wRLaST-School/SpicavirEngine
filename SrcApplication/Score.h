#pragma once
class Score
{
public:
	int32_t totDamage = 0;
	
	static void Load();
	void Init();
	void Update();
	void Draw();

	const float gradeMax = 21000.0f;
	const float gradeS = gradeMax * 13 / 16;
	const float gradeA = gradeMax * 10 / 16;
	const float gradeB = gradeMax * 7 / 16;
	const float gradeC = gradeMax * 4 / 16;

private:
};

