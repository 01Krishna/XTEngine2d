#include "BatchRenderer.h"

XTEngine2d::BatchRenderer::BatchRenderer()
{
}

XTEngine2d::BatchRenderer::~BatchRenderer()
{
    delete[] VertexBufferBase;
}

void XTEngine2d::BatchRenderer::Init()
{

    VAO = 0;
    VBO = 0;
    IBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MaxQuads, nullptr, GL_DYNAMIC_DRAW);


    uint32_t indices[MaxQuads * 6];
    uint32_t offset = 0;

    for (size_t i = 0; i < MaxQuads * 6; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned char whitePixel[] = { 255,255,255,255 };

    glGenTextures(1, &m_WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);

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

}

void XTEngine2d::BatchRenderer::BeginBatch()
{
	VertexBufferPtr = VertexBufferBase;
	QuadCount = 0;
}

void XTEngine2d::BatchRenderer::SubmitQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec4 uv)
{
    assert(QuadCount < MaxQuads);
   
    std::array<XTEngine2d::Vertex, 4> tempVertices =
    {
        Vertex{glm::vec2(pos.x, pos.y), color,glm::vec2(uv.x,uv.y)},
        Vertex{glm::vec2(pos.x + size.x, pos.y), color, glm::vec2(uv.z, uv.y)},
        Vertex{glm::vec2(pos.x + size.x, pos.y + size.y), color, glm::vec2(uv.z, uv.w)},
        Vertex{glm::vec2(pos.x, pos.y + size.y), color, glm::vec2(uv.x, uv.w)}
	};



	*VertexBufferPtr++ = tempVertices[0];       
	*VertexBufferPtr++ = tempVertices[1];
	*VertexBufferPtr++ = tempVertices[2];
	*VertexBufferPtr++ = tempVertices[3];

	QuadCount++;
                    
    m_Stats.QuadCount++;
    m_Stats.VertexCount += 4;
    m_Stats.IndexCount += 6;

}

void XTEngine2d::BatchRenderer::EndBatch()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * QuadCount, VertexBufferBase);
}


void XTEngine2d::BatchRenderer::Flush(std::shared_ptr<Texture> texture, glm::mat4 view_projection)
{
    m_Stats.DrawCalls++;

    glBindVertexArray(VAO);

    m_Shader.use();
    
    unsigned int textureId = texture.get()->GetId() ? texture.get()->GetId() : m_WhiteTexture;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
	m_Shader.setInt("ourTexture", 0);

	m_Shader.setMat4("viewProjection", glm::value_ptr(view_projection));

    glDrawElements(GL_TRIANGLES, QuadCount * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
