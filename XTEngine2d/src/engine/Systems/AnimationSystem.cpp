#include "../Scene.h"
#include "../Renderer.h"
#include "../Camera.h"
#include "../Animation.h"
#include "../AnimationStateMachine.h"
#include "AnimationSystem.h"

namespace XTEngine2d
{
	void AnimationSystem(XTEngine2d::Scene* scene, float deltaTime)
	{

		const auto& entities = scene->m_Registry.View<XTEngine2d::Animation, XTEngine2d::Sprite>();


		for (auto& entity : entities)
		{
			if (scene->m_Registry.HasComponent<Animation>(entity))
			{
				auto& animation = scene->m_Registry.GetComponent<Animation>(entity);
				if (scene->m_Registry.HasComponent<AnimationStateMachine>(entity))
				{
					auto& stateMachine = scene->m_Registry.GetComponent<AnimationStateMachine>(entity);

					animation.m_Looping = stateMachine.m_CurrentState.second.looping;
					animation.m_StartFrame = stateMachine.m_CurrentState.second.startingFrame;
				}

				if (scene->m_Registry.HasComponent<Sprite>(entity))
				{
					auto& sprite = scene->m_Registry.GetComponent<Sprite>(entity);
					auto& sheet = scene->m_Registry.GetComponent<SpriteSheet>(entity);

					if (!animation.m_Playing)
						return;

					animation.m_Timer += deltaTime;

					animation.m_Fps = glm::clamp(animation.m_Fps, 1.0f, 2000.0f);

					float frameTime = 1.f / animation.m_Fps;
				
					if (animation.m_Timer >= frameTime)
					{
						animation.m_Timer -= frameTime;
						animation.m_CurrentFrame++;

						if (animation.m_CurrentFrame > animation.m_EndFrame)
						{
							if (animation.m_Looping)
								animation.m_CurrentFrame = animation.m_StartFrame;
							else
							{
								animation.m_CurrentFrame = animation.m_EndFrame;
								animation.m_Timer = 0.f;
							}
						}

						
						if (animation.m_RowOrColumnAnchor) // true = row, false = column 
						{
							int Animationrow = animation.m_StartFrame / sheet.m_Columns;
							int Animationcolumn = animation.m_CurrentFrame % sheet.m_Columns;
							Animationrow += animation.m_StartingRow;
							sprite.uv = sheet.GetUVFromRowColumn(Animationcolumn, Animationrow);
						}
						else
						{
							int Animationcolumn = animation.m_StartFrame / sheet.m_Columns;
							int Animationrow = animation.m_CurrentFrame % sheet.m_Columns;
							Animationcolumn += animation.m_StartingColumn;
							sprite.uv = sheet.GetUVFromRowColumn(Animationcolumn, Animationrow);
						}
					}

				}
			}
		}
	}
};