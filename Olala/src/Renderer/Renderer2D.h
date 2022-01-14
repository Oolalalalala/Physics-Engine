#pragma once

#include "Texture2D.h"
#include "Camera.h"

#include "glm/glm.hpp"

namespace Olala {

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<Texture2D> texture, glm::vec4 color);


		static void DrawCircle(glm::vec2 position, float radius, float thickness = 0.05f, glm::vec4 color = glm::vec4(1.0f));
		static void DrawLine();

	private:
		static uint32_t BatchQuads();
		static void FlushQuads(uint32_t indexCount);

		static uint32_t BatchCircles();
		static void FlushCircles(uint32_t indexCount);

		static void Reset();

		static uint32_t WriteQuadDataToBuffer(std::vector<uint32_t>& quadIndices, uint32_t textureIndex);

	private:
		struct CameraData
		{
			glm::mat4 m_ViewProjectionMatrix;
		};

		static CameraData s_CameraData;
	};

}