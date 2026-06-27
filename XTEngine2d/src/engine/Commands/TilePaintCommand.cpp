#include "TilePaintCommand.h"

void XTEngine2d::TilePaintCommand::Execute()
{
	auto& tilemap = m_Scene->m_Registry.GetComponent<TileMap>(m_Entity);

	for (const auto& [index, selectedTile] : m_ModifiedTiles)
	{
		if (tilemap.tiles[index] == selectedTile)
			continue;

		tilemap.tiles[index] = selectedTile;
	}
}

void XTEngine2d::TilePaintCommand::Undo()
{
	auto& tilemap = m_Scene->m_Registry.GetComponent<TileMap>(m_Entity);

	for (const auto& [index, originalTile] : m_OriginalTiles)
	{
		tilemap.tiles[index] = originalTile;
	}
}
