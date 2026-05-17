#pragma once 

#include <glad/glad.h>


class IndexBufferObject
{
public:
	unsigned int ID;

	IndexBufferObject()
	{
		glGenBuffers(1, &ID);
	}

	const void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void SetBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}

	const void UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};