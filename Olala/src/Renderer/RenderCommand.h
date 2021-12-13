#pragma once

#include "VertexArray.h"
#include "Framebuffer.h"
#include "glm/glm.hpp"

namespace Olala {

	class RenderCommand
	{
	public:
		static void Init();

		static void SetClearColor(const glm::vec4 color);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void SetRenderTarget(Ref<Framebuffer> renderTarget);

		static void Clear();
		static void DrawIndexed(Ref<VertexArray>& vertexArray, uint32_t IndexCount = 0);
	};

}