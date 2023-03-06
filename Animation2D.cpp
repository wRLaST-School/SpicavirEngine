#include "stdafx.h"
#include "Animation2D.h"

void Animation2D::Register(vector<TextureKey> texKeys, int cooltime, bool loop, AnimKey key)
{
	animMap.insert(pair<AnimKey, AnimData>(key, { texKeys, 0, (int)texKeys.size(), 0, cooltime, loop }));

	if (!current) current = &animMap.find(key)->second;
}

void Animation2D::SetDefaultKey(AnimKey key)
{
	default = key;
}

void Animation2D::Update()
{
	current->timer++;
	if (current->timer >= current->cooltime)
	{
		current->timer = 0;

		current->currentIndex++;
		if (current->currentIndex >= current->maxIndex)
		{
			if (current->loop)
			{
				current->currentIndex = 0;
			}
			else
			{
				animationEnd = true;
				current->currentIndex = current->maxIndex - 1;
			}
		}
	}
}

void Animation2D::Set(AnimKey key)
{
	current = &animMap.find(key)->second;
	current->currentIndex = 0;
	current->timer = 0;
	animationEnd = false;
}

TextureKey Animation2D::Get()
{
	return current->textures.at(current->currentIndex);
}

bool Animation2D::isAnimationEnd()
{
	return animationEnd;
}
