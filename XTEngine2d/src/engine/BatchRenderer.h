#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp> 
#include <array>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Shader.h"
#include "Sprite.h"
#include "Log.h"
#include "Textures.h"


namespace XTEngine2d {


	struct Vertex
	{
		glm::vec2 position;
		glm::vec4 color;
		glm::vec2 uv;
	};

	struct RendererStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t VertexCount = 0;
		uint32_t IndexCount = 0;

		void Reset()
		{
			DrawCalls = 0;
			QuadCount = 0;
			VertexCount = 0;
			IndexCount = 0;
		}
	};

	class BatchRenderer
	{
	public:
		BatchRenderer();
		~BatchRenderer();

	public:
		unsigned int VBO;
		unsigned int VAO;
		unsigned int IBO;

		unsigned int m_WhiteTexture;

		Shader m_Shader;

		RendererStats m_Stats;
		constexpr static unsigned int MaxQuads = 10000;


		Vertex* VertexBufferBase = new Vertex[MaxQuads*4];
		Vertex* VertexBufferPtr;
		uint32_t QuadCount;


	public:
		void Init();
		void BeginBatch();
		void SubmitQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec4 uv);
		void EndBatch();	
		void Flush(std::shared_ptr<Texture> texture, glm::mat4 view_projection);

	};
}