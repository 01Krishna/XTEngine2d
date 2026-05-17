#pragma once 

#include <glad/glad.h>
#include "Core.h"
#include "stb_image.h"
#include <glm.hpp>
#include <iostream>

namespace XTEngine2d
{

	class XT_API Texture
	{
	public:
		unsigned int ID = 0;
		glm::vec2 Texture_Size = { 10, 10 };
		std::string path;

		Texture()
		{
		}

		~Texture()
		{
			glDeleteTextures(1, &ID);
		}

		unsigned int GetId()
		{
			return ID;
		}

		int GetWidth()
		{
			return Texture_Size.x;
		}

		int GetHeight()
		{
			return Texture_Size.y;
		}


		std::string GetPath()
		{
			return path;
		}

		void LoadTexture(const char* fileName)
		{
			path = fileName;

			glGenTextures(1, &ID);
			glBindTexture(GL_TEXTURE_2D, ID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


			glBindTexture(GL_TEXTURE_2D, ID);

			int width;
			int height;
			int nrChannels;
			stbi_set_flip_vertically_on_load(false);

			unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

			if (data)
			{
				GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
				//GLenum format = GL_RGB;	
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed To Load Texture" << std::endl;
			}

			Texture_Size.x = width;
			Texture_Size.y = height;

			stbi_image_free(data);
		}

		void GenerateTexture(unsigned char* whitePixel)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				1,
				1,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				whitePixel
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		};


		const void Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, ID);
		}

		const void UnBind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	};


	inline Texture& WhiteTexture()
	{
		static Texture tex;
		static bool initialized = false;
		if (!initialized)
		{
			unsigned char whitePixel[] = { 255,255,255,255 };
			tex.GenerateTexture(whitePixel);
			initialized = true;
		}
		return tex;
	}
};