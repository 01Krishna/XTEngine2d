#include "FrameBuffer.h"

void XTEngine2d::Framebuffer::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // Color texture
    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
        m_Width, m_Height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_ColorAttachment,
        0);

    // Depth buffer
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

    glRenderbufferStorage(GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        m_Width, m_Height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
        != GL_FRAMEBUFFER_COMPLETE)
    {
		XT_CORE_ERROR("Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void XTEngine2d::Framebuffer::Resize(uint32_t width, uint32_t height)
{

    if (width == 0 || height == 0)
        return;

    if (m_Width == width && m_Height == height)
        return;

    m_Width = width;
    m_Height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // ===== COLOR ATTACHMENT =====
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        m_Width,
        m_Height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    // ===== DEPTH/STENCIL =====
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        m_Width,
        m_Height
    );

    // ===== SAFETY RECHECK =====
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        XT_CORE_ERROR("Framebuffer resize failed!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void XTEngine2d::Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void XTEngine2d::Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
