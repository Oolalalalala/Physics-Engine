#pragma once

namespace Olala {

	struct FramebufferSpecs
	{
		uint32_t Width = 960, Height = 540;
		GLenum ColorBufferInternalFormat = GL_RGBA8, ColorBufferDataFormat = GL_RGBA;
	};

	class Framebuffer
	{
	public:
		Framebuffer(FramebufferSpecs specs);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		void Invalidate();
		void Resize(uint32_t width, uint32_t height);

		const FramebufferSpecs& GetSpecs() { return m_Specs; }

		uint32_t GetColorBufferRendererId() const { return m_ColorBufferRendererID; }

		static Ref<Framebuffer> Create(FramebufferSpecs specs);

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorBufferRendererID;
		uint32_t m_DepthBufferRendererID;
		FramebufferSpecs m_Specs;
	};
}