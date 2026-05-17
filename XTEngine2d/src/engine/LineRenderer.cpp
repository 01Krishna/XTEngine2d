#include "LineRenderer.h"

namespace XTEngine2d
{
    void LineRenderer::Init()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * 4,
            nullptr,
            GL_DYNAMIC_DRAW);

        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            (void*)0);

        glEnableVertexAttribArray(0);
    }

    void LineRenderer::Shutdown()
    {

    }

    void LineRenderer::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color, glm::mat4 viewProjection)
    {
        float vertices[] =
        {
            start.x, start.y,
            end.x, end.y
        };

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(vertices),
            vertices);

        m_Shader.use();

        m_Shader.setMat4(
            "viewProjection",
            glm::value_ptr(viewProjection));

        m_Shader.setVec4(
            "color",
            glm::value_ptr(color));

        glDrawArrays(GL_LINES, 0, 2);
    }
}