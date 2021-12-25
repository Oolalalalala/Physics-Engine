#pragma once

#include <unordered_map>
#include "Core/Base.h"

namespace Olala {

	template<typename T>
	class AssetPool
	{
	public:
		AssetPool() = default;
		~AssetPool();

		void Add(const std::string& name, Ref<T> asset);
		Ref<T> Get(const std::string& name);
		void Remove(const std::string& name);
		const std::unordered_map<std::string, Ref<T>>& GetAll() const;

	private:
		std::unordered_map<std::string, Ref<T>> m_Assets;
	};

	template<typename T>
	inline AssetPool<T>::~AssetPool()
	{
		for (auto& asset : m_Assets)
		{
			asset.second.reset();
		}
	}

	template<typename T>
	inline void AssetPool<T>::Add(const std::string& name, Ref<T> asset)
	{
		m_Assets[name] = asset;
	}

	template<typename T>
	inline Ref<T> AssetPool<T>::Get(const std::string& name)
	{
		if (m_Assets.find(name) != m_Assets.end())
			return m_Assets[name];
		return nullptr;
	}

	template<typename T>
	inline void AssetPool<T>::Remove(const std::string& name)
	{
		m_Assets.erase(name);
	}

	template<typename T>
	inline const std::unordered_map<std::string, Ref<T>>& AssetPool<T>::GetAll() const
	{
		return m_Assets;
	}

}