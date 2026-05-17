#pragma once
#include "Utility.h"
#include "stb_image.h"

namespace XTEngine2d
{

	class XT_API Sprite
	{
	public:
		Sprite() {}
		Sprite(std::shared_ptr<XTEngine2d::Texture> tex, glm::vec4 Iuv)
			:texture(tex), uv(Iuv)
		{

		}
		std::shared_ptr<XTEngine2d::Texture> texture;
		glm::vec4 uv = glm::vec4(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
		std::string path;

		void Setuv(glm::vec4 Iuv)
		{
			uv = Iuv;
		}

		void SetColor(glm::vec4 Icolor)
		{
			color = Icolor;
		}

		std::string GetPath()
		{
			return texture->GetPath();
		}

	};
}