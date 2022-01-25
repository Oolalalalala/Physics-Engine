#pragma once

#include "Panel.h"
#include <Olala.h>

class AssetPanel : public Panel
{
public:
	AssetPanel(Olala::Ref<Olala::AssetManager> assetManager);
	~AssetPanel();

	void SetAssetManager(Olala::Ref<Olala::AssetManager> assetManager);

	void OnUpdate(float dt) override {}
	void OnImGuiRender() override;

private:
	Olala::Ref<Olala::AssetManager> m_AssetManager;

	enum class AssetDirectory {
		Root = 0, Texture
	};
	AssetDirectory m_CurrentDirectory = AssetDirectory::Root;
};