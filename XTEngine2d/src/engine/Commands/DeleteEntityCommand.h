#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../EntitySnapshot.h"

namespace XTEngine2d
{


	class DeleteEntityCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity;
		EntitySnapshot m_EntitySnapshot;

	public:
		DeleteEntityCommand(Scene* scene, Entity& entity)
		{
			m_Scene = scene;
			m_Entity = entity;
			CommandName = "DeleteEntityCommand";
			m_EntitySnapshot.TakeEntitySnapshot(entity, scene);
		};
		void Execute() override;
		void Undo() override;

	};

}