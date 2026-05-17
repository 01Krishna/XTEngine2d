#pragma once

#include <unordered_map>
#include <string>


enum AnimationState
{
	Idle = 0,
	Run,
	Walk,
	Jump, 
	Interacting
};


struct AnimationData
{
	int startFrame;
	int frameCount;
	float frameTime;
	bool loop;
};


class Animator
{
public:
	std::unordered_map<std::string, AnimationData> Animations;
	std::string currentAnimation;
	float timer;
	int currentFrame;

	SpriteRenderer* renderer = nullptr;

	void AddAnimation(std::string& name, AnimationData& data)
	{
		Animations[name] = data;
	}


	void PlayAnimation(std::string& name)
	{
		if (currentAnimation == name) return;
		
		currentAnimation = name;	
		timer = 0.0f;
		currentFrame = 0;
	}


	void Update(float dt)
	{
		if (currentAnimation.empty()) return;

		timer += dt;

		AnimationData& data = Animations[currentAnimation];

		if (timer >= data.frameTime)
		{
			timer = 0.0f;
			currentFrame++;

			if (currentFrame >= data.frameCount)
			{
				if (data.loop)
				{
					currentFrame = 0;
				}
				else
					currentFrame = data.frameCount - 1;
			}

			renderer->SetFrame(anim.startFrame + currentFrame);
		}

	}


	void SetRenderer(SpriteRenderer* r)
	{
		renderer = r;
	}

};