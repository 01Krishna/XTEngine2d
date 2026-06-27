#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"

namespace XTEngine2d
{


	class MoveEntityCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity;
		glm::vec2 m_NewPosition;
		glm::vec2 m_OldPosition;

	public:
		MoveEntityCommand(Scene* scene, Entity ent, glm::vec2 newPosition, glm::vec2 oldPosition)
		{
			m_Scene = scene;
			m_Entity = ent;
			m_NewPosition = newPosition;
			m_OldPosition = oldPosition;
			CommandName = "MoveEntityCommand";

		};
		void Execute() override;
		void Undo() override;

	};

}