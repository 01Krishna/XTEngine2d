#pragma once

#include "Textures.h"

namespace XTEngine2d
{
    class SpriteSheet
    {
    private:
        Texture* m_Texture;
    public:
        int m_SpriteWidth;
        int m_SpriteHeight;
    public:
        int m_Columns;
        int m_Rows;

		int m_SelectedColumn = 0;
		int m_SelectedRow = 0;

    public:
        SpriteSheet() = default;
        SpriteSheet(Texture* texture, int spriteWidth, int spriteHeight)
            : m_Texture(texture),
            m_SpriteWidth(spriteWidth),
            m_SpriteHeight(spriteHeight)
        {
            m_Columns = texture->GetWidth() / spriteWidth;
            m_Rows = texture->GetHeight() / spriteHeight;


        }

        void SetTexture(Texture* texture, int spriteWidth, int spriteHeight)
        {
            m_Texture = texture;
            m_SpriteWidth = spriteWidth;
            m_SpriteHeight = spriteHeight;

            m_Columns = texture->GetWidth() / spriteWidth;
            m_Rows = texture->GetHeight() / spriteHeight;
        }

        void SetSpriteDimension(int spriteWidth, int spriteHeight)
        {
            m_SpriteWidth = spriteWidth;
            m_SpriteHeight = spriteHeight;

            if (spriteWidth && spriteHeight != 0)
            {
                m_Columns = m_Texture->GetWidth() / spriteWidth;
                m_Rows = m_Texture->GetHeight() / spriteHeight;
            }
        }

        glm::vec4 GetUV(int column, int row)
        {
                    
            float texWidth = (float)m_Texture->GetWidth();
            float texHeight = (float)m_Texture->GetHeight();

            float x = column * m_SpriteWidth;
            float y = row * m_SpriteHeight;

            float u1 = x / texWidth;
            float v1 = y / texHeight;
            float u2 = (x + m_SpriteWidth) / texWidth;
            float v2 = (y + m_SpriteHeight) / texHeight;

            return { u1, v1, u2, v2 };
        }

        glm::vec4 GetUVFromFrame(int frame)
        {
            int column = frame % m_Columns;
            int row = frame / m_Columns;

            return GetUV(column, row);
        }

        glm::vec4 GetUVFromRowColumn(int column, int row)
        {
            return GetUV(column, row);
		}

        glm::vec2 GetSelectedRowColumn()
        {
			return { (float)m_SelectedColumn, (float)m_SelectedRow };
        }
    };
};