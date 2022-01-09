#pragma once

namespace Olala {

	class Texture2D
	{
	public:
		Texture2D(const std::string path);
		~Texture2D();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		void Bind(uint32_t slot = 0) const;

		bool IsLoaded() const { return m_IsLoaded; }
		uint32_t GetRendererID() const { return m_RendererID; }

		bool operator==(const Texture2D& other)
		{
			return m_RendererID == other.m_RendererID;
		}

		static Ref<Texture2D> Create(const std::string& filepath);

	private:
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};

}