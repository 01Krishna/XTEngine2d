#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../EntitySnapshot.h"

namespace XTEngine2d
{


	class DuplicateEntityCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity;
		EntitySnapshot m_EntitySnapshot;


	public:
		DuplicateEntityCommand(Scene* scene, Entity& entity)
		{
			m_Scene = scene;
			CommandName = "DuplicateEntityCommand";

			m_EntitySnapshot.TakeEntitySnapshot(entity, scene);                          

		};

		void Execute() override
		{
			m_Entity = m_Scene->m_Registry.CreateEntity();

			m_EntitySnapshot.RetriveSnapshotData(m_Entity, m_Scene);
		};

		void Undo() override
		{
			m_Scene->m_Registry.DestroyEntity(m_Entity);
		};

		Entity GetEntity() { return m_Entity; };

	};

}