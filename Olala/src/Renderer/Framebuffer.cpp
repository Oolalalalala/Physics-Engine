#include "pch.h"
#include "Framebuffer.h"

namespace Olala {

	Ref<Framebuffer> Framebuffer::Create(FramebufferSpecs specs)
	{
		return CreateRef<Framebuffer>(specs);
	}

	Framebuffer::Framebuffer(FramebufferSpecs specs)
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBufferRendererID);
		glTextureStorage2D(m_ColorBufferRendererID, 1, specs.ColorBufferInternalFormat, specs.Width, specs.Height);
		glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ColorBufferRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBufferRendererID);
		glTextureStorage2D(m_DepthBufferRendererID, 1, GL_DEPTH24_STENCIL8, specs.Width, specs.Height);
		glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_DepthBufferRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferRendererID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferRendererID, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			CORE_LOG_ERROR("Framebuffer creation incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorBufferRendererID);
		glDeleteTextures(1, &m_DepthBufferRendererID);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}