#pragma once

#include <filesystem>

#include "AssetPool.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Shader.h"

namespace Olala {

	namespace fs = std::filesystem;

	class Scene;

	class AssetManager
	{
	public:
		AssetManager() = default;
		AssetManager(const fs::path& directoryPath);
		~AssetManager();

		void Load(const fs::path& directoryPath);
		void Unload();

		template<typename T>
		AssetPool<T>& GetPool();

	private:
		struct Pools
		{
			AssetPool<Texture2D> Texture;
		};
		Pools m_Pools;
	};

}