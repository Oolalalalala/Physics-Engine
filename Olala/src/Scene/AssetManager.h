#pragma once

#include <filesystem>

#include "AssetPool.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Shader.h"

namespace Olala {

	class AssetManager
	{
	public:
		AssetManager() = default;
		AssetManager(const std::string& folderPath);
		~AssetManager();

		void Load(const std::string& folderPath);
		void Unload();

		template<typename T>
		AssetPool<T>& GetPool();

	private:
		std::filesystem::path m_FolderPath;
		
		struct Pools
		{
			AssetPool<Texture2D> Texture;
			AssetPool<Shader> Shader;
		};
		Pools m_Pools;
	};

}