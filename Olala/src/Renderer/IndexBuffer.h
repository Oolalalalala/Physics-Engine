#pragma once

namespace Olala {

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}