#pragma once
#include "Shader.h"
#include "QuadMesh.h"
#include "Sprite.h"
#include "Utility.h"

namespace XTEngine2d
{
	class Renderer
	{
	public:
		glm::mat4 m_ViewProjection;
		unsigned int m_WhiteTexture;
	public:
		void Init();

		void Draw(Shader& shader, Sprite& sprite, Transform& transform);
		void DrawQuad(Shader& shader, Texture* texture, glm::vec2 pos, glm::vec4 uv, glm::vec2 size);
		void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
	
		void BeginScene(const glm::mat4& vp);
	};
};