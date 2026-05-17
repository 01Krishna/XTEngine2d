#pragma once

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"


struct Vertex_Array_Buffer
{
	VertexArrrayObject VAO;
	VertexBufferObject VBO;
	IndexBufferObject IBO;
};


class QuadMesh
{
public:
	static unsigned int VAO;
	static unsigned int VBO;
	static unsigned int IBO;

	static void Init();
};