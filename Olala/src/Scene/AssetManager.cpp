#include "pch.h"
#include "AssetManager.h"

namespace Olala {

	AssetManager::AssetManager(const std::string& folderPath)
	{
		Load(folderPath);
	}

	AssetManager::~AssetManager()
	{
		Unload();
	}

	void AssetManager::Load(const std::string& folderPath)
	{
		for (auto folder : std::filesystem::directory_iterator(folderPath))
		{
			std::string name = folder.path().stem().string();
			if (name == "Texture")
			{
				for (auto texture : std::filesystem::directory_iterator(folder))
				{
					m_Pools.Texture.Add(texture.path().filename().string(), Texture2D::Create(texture.path().string()));
				}
			}
		}

		m_FolderPath = folderPath;
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