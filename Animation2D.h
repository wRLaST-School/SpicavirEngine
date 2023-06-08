#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>

typedef std::string AnimKey;

class Animation2D
{
public:
	Animation2D(){};
	//�A�j���[�V������o�^�B���o�^�Ȃ珉���A�j���[�V�����ɁB
	void Register(vector<TextureKey> texKeys, int32_t cooltime, bool loop, AnimKey key);
	void SetDefaultKey(AnimKey key);
	void Update();
	void Set(AnimKey key); // animationEnd��������

	TextureKey Get();
	bool isAnimationEnd();
private:
	struct AnimData {
		vector<TextureKey> textures{};
		int32_t currentIndex = 0;
		int32_t maxIndex = 0;
		int32_t timer = 0;
		int32_t cooltime = 0;
		bool loop = false;
	};

	AnimData* current = nullptr;
	AnimKey default = "__Undefined";
	bool animationEnd = false;

	
	map<AnimKey, AnimData> animMap;
};

