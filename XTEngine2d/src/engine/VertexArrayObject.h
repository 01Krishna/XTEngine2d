#pragma once
#include <glad/glad.h>


class VertexArrrayObject
{
public:
	unsigned int ID;

	VertexArrrayObject()
	{
		glGenVertexArrays(1, &ID);
	}

	const void Bind() const 
	{
		glBindVertexArray(ID);
	}

	void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
	{
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void EnableVertexAttribArray(GLuint index)
	{
		glEnableVertexAttribArray(index);
	}

	void DisableVertexAttribArray(GLuint index)
	{
		glDisableVertexAttribArray(index);
	}

	const void UnBind() const
	{
		glBindVertexArray(0);
	}
	
	const void DeleteVertexArray() const
	{
		glDeleteVertexArrays(1, &ID);
	}

};