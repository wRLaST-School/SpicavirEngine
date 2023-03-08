#pragma once
class Toolbar
{
private:
	enum class MenuType {
		File,
		Tool
	};

public:
	static void ProcessMessage(WPARAM wp);

	static HMENU hm;
};

