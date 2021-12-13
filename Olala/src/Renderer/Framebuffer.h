#pragma once

namespace Olala {

	struct FramebufferSpecs
	{
		uint32_t Width, Height;
		GLenum ColorBufferInternalFormat = GL_RGBA8, ColorBufferDataFormat = GL_RGBA;
	};

	class Framebuffer
	{
	public:
		Framebuffer(FramebufferSpecs specs);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetColorBufferRendererId() const { return m_ColorBufferRendererID; }

		static Ref<Framebuffer> Create(FramebufferSpecs specs);

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorBufferRendererID;
		uint32_t m_DepthBufferRendererID;
	};
}