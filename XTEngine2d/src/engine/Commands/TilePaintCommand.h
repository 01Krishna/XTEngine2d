#pragma once
#include "../ICommand.h"
#include "../Utility.h"
#include "../Scene.h"
#include "../TileMap.h"

namespace XTEngine2d
{


	class TilePaintCommand :public ICommand
	{

	private:
		Scene* m_Scene;
		Entity m_Entity;
		std::unordered_map<int, int> m_ModifiedTiles;
		std::unordered_map<int, int> m_OriginalTiles;

	public:
		TilePaintCommand(Scene* scene, Entity ent, std::unordered_map<int, int> &modifiedTiles, std::unordered_map<int, int>& originalTiles)
		{
			m_Scene = scene;
			m_Entity = ent;	
			m_ModifiedTiles = modifiedTiles;
			m_OriginalTiles = originalTiles;
			CommandName = "TilePaintCommand";
		};
		void Execute() override;
		void Undo() override;

	};

}