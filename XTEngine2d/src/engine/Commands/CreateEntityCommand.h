#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../Log.h"
namespace XTEngine2d
{


	class CreateEntityCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity = 0;

	public:
		CreateEntityCommand(Scene* scene)
		{
			m_Scene = scene;
			CommandName = "CreateEntityCommand";
		};
		void Execute() override;
		void Undo() override;
		Entity GetCreatedEntity() { return m_Entity; };

	};

}