#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../Log.h"
#include "../AssetManager.h"
#include "../EntitySnapshot.h"

namespace XTEngine2d
{


	class CreateSpriteEntityCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		std::string m_Path;
		Entity m_Entity = 0;
		EntitySnapshot m_EntitySnapshot;

	public:
		CreateSpriteEntityCommand(Scene* scene, std::string path)
		{
			m_Scene = scene;
			m_Path = path;
			CommandName = "CreateSpriteEntityCommand";	
		};
		void Execute() override;
		void Undo() override;
		Entity GetCreatedEntity() { return m_Entity; };

	};

}