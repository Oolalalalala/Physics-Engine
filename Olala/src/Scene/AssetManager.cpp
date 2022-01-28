#include "pch.h"
#include "AssetManager.h"

#include "Scene.h"
#include "SceneSerializer.h"

namespace Olala {

	AssetManager::AssetManager(const fs::path& sceneFilePath)
	{
		Load(sceneFilePath);
	}

	AssetManager::~AssetManager()
	{
		Unload();
	}

	void AssetManager::Load(const fs::path& directoryPath)
	{
		for (auto folder : fs::directory_iterator(directoryPath))
		{
			std::string name = folder.path().stem().string();
			if (name == "Texture")
			{
				for (auto texture : fs::directory_iterator(folder))
				{
					m_Pools.Texture.Add(texture.path().filename().string(), Texture2D::Create(texture.path().string()));
				}
			}
		}
	}

	void AssetManager::Unload()
	{
	}

	template<typename T>
	AssetPool<T>& AssetManager::GetPool()
	{
		static_assert(false);
	}
	template<>
	AssetPool<Texture2D>& AssetManager::GetPool()
	{
		return m_Pools.Texture;
	}

}