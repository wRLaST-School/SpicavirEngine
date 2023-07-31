#pragma once
#include <Essentials.h>
#include <SpTextureManager.h>

typedef std::string AnimKey;

class Animation2D
{
public:
	Animation2D(){};
	//�A�j���[�V������o�^�B���o�^�Ȃ珉���A�j���[�V�����ɁB
	void Register(std::vector<TextureKey> texKeys, int32_t cooltime, bool loop, const AnimKey& key);
	void SetDefaultKey(const AnimKey& key);
	void Update();
	void Set(const AnimKey& key); // animationEnd��������

	TextureKey Get();
	bool isAnimationEnd();
private:
	struct AnimData {
		std::vector<TextureKey> textures{};
		int32_t currentIndex = 0;
		int32_t maxIndex = 0;
		int32_t timer = 0;
		int32_t cooltime = 0;
		bool loop = false;
	};

	AnimData* current_ = nullptr;
	AnimKey default_ = "__Undefined";
	bool animationEnd_ = false;

	
	std::map<AnimKey, AnimData> animMap;
};
