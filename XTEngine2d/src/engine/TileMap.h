#pragma once
#include <memory>
#include "Textures.h"


namespace XTEngine2d
{
	struct TileMap
	{
		int height = 8;
		int width = 8;

		int gridSize = 32;

		std::vector<int> tiles;

		bool isAnimated = false;
		int m_FrameCount = 0;
		float animationSpeed = 1.0f;

		std::shared_ptr<Texture> texture;
	};
}