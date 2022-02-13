#include "AssetPanel.h"

AssetPanel::AssetPanel(Olala::Ref<Olala::AssetManager> assetManager)
	: Panel("Asset")
{
	m_AssetManager = assetManager;
}

AssetPanel::~AssetPanel()
{
}

void AssetPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		
		switch (m_CurrentDirectory)
		{
			case AssetDirectory::Root:     
				ImGui::ArrowButton("Root asset", ImGuiDir_Left);  
				ImGui::Text("Scene");
				break;
			case AssetDirectory::Texture:  
				if (ImGui::ArrowButton("Root asset", ImGuiDir_Left))
					m_CurrentDirectory = AssetDirectory::Root;
				ImGui::Text("Scene  >  Texture");  
				break;
		}

		ImGui::EndMenuBar();



		if (m_AssetManager)
		{
			switch (m_CurrentDirectory)
			{
				case AssetDirectory::Root:
				{
					if (ImGui::Button("Texture")) m_CurrentDirectory = AssetDirectory::Texture;
					break;
				}
				case AssetDirectory::Texture:
				{
					auto& textures = m_AssetManager->GetPool<Olala::Texture2D>().GetAll();
					ImGui::Columns(8, "texture_column", false);
					for (auto& [name, texture] : textures)
					{
						ImGui::SetCursorPosX(ImGui::GetColumnOffset() + ImGui::GetColumnWidth() * 0.5 - 30.f);
						ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2(60.f, 60.f), ImVec2(0, 1), ImVec2(1, 0));

						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("Texture", &texture, sizeof(texture));
							ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2(60.f, 60.f), ImVec2(0, 1), ImVec2(1, 0));
							ImGui::EndDragDropSource();
						}
						ImGui::PopStyleVar();

						ImGui::SetCursorPosX(ImGui::GetColumnOffset() + ImGui::GetColumnWidth() * 0.5 - ImGui::CalcTextSize(name.c_str()).x * 0.5);
						ImGui::Text(name.c_str());

						ImGui::NextColumn();
					}
					
					break;
				}
			}
		}
		m_IsFocused = ImGui::IsWindowFocused();

		ImGui::End();
	}
}

void AssetPanel::SetAssetManager(Olala::Ref<Olala::AssetManager> assetManager)
{
	m_AssetManager = assetManager;
}