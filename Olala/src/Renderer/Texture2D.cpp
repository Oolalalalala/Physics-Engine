#include "pch.h"
#include "Texture2D.h"

#include <stb_image/stb_image.h>

namespace Olala {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		return CreateRef<Texture2D>(path);
	}

	Texture2D::Texture2D(const std::string path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			CORE_LOG_WARN("Cannot load texture 2D, path : {}", path);
			return;
		}

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		m_IsLoaded = true;
		m_Width = width, m_Height = height;

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glBindTextureUnit(slot, m_RendererID);
	}

}