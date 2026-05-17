#pragma once 

#include <glad/glad.h>

class VertexBufferObject
{
public:
	unsigned int ID;

	VertexBufferObject()
	{
		glGenBuffers(1, &ID);
	}

	const void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void SetBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
	{
		glBufferData(target, size, data, usage);
	}

	void UpdateBufferData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
	{
		Bind();
		glBufferSubData(target, offset, size, data);
		UnBind();
	}

	const void UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const void DeleteVertexBuffer() const
	{
		glDeleteBuffers(1, &ID);
	}
};