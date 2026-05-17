#pragma once
#include <glad/glad.h>
#include <cstdint>
#include "Log.h"

namespace XTEngine2d
{

    class Framebuffer
    {
    public:
        void Init(uint32_t width, uint32_t height);
        void Resize(uint32_t width, uint32_t height);

        void Bind();
        void Unbind();

        uint32_t GetColorAttachment() const { return m_ColorAttachment; }
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

    private:
        uint32_t m_FBO = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_RBO = 0;

        uint32_t m_Width = 0, m_Height = 0;
    };
}