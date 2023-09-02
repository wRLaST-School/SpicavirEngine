#include "stdafx.h"
#include "Animation2D.h"

void Animation2D::Register(std::vector<TextureKey> texKeys, int32_t cooltime, bool loop, const AnimKey& key)
{
	animMap.insert(std::pair<AnimKey, AnimData>(key, { texKeys, 0, (int32_t)texKeys.size(), 0, cooltime, loop }));

	if (!current_) current_ = &animMap.find(key)->second;
}

void Animation2D::SetDefaultKey(const AnimKey& key)
{
	default_ = key;
}

void Animation2D::Update()
{
	current_->timer++;
	if (current_->timer >= current_->cooltime)
	{
		current_->timer = 0;

		current_->currentIndex++;
		if (current_->currentIndex >= current_->maxIndex)
		{
			if (current_->loop)
			{
				current_->currentIndex = 0;
			}
			else
			{
				animationEnd_ = true;
				current_->currentIndex = current_->maxIndex - 1;
			}
		}
	}
}

void Animation2D::Set(const AnimKey& key)
{
	current_ = &animMap.find(key)->second;
	current_->currentIndex = 0;
	current_->timer = 0;
	animationEnd_ = false;
}

TextureKey Animation2D::Get()
{
	return current_->textures.at(current_->currentIndex);
}

bool Animation2D::isAnimationEnd()
{
	return animationEnd_;
}
