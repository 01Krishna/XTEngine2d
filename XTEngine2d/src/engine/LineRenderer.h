#pragma once
#include <glad/glad.h>
#include "Utility.h"

namespace XTEngine2d
{
    class LineRenderer
    {
    public:
        void Init();
        void Shutdown();

        void DrawLine(
            glm::vec2 start,
            glm::vec2 end,
            glm::vec4 color,
            glm::mat4 viewProjection);
    public:
        Shader m_Shader;
    private:
        unsigned int m_VAO;
        unsigned int m_VBO;

    };
}