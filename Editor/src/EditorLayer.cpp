#include "EditorLayer.h"


void EditorLayer::OnAttach()
{
	m_Scene = Olala::CreateRef<Olala::Scene>();

	// Editor camera
	Olala::Entity editorCamera = m_Scene->CreateEntity("Editor Camera");
	auto& cameraComponent = editorCamera.AddComponent<Olala::CameraComponent>(Olala::CreateRef<Olala::OrthographicCamera>());
	Olala::FramebufferSpecs specs;
	specs.Width = 800, specs.Height = 600;
	cameraComponent.RenderTarget = Olala::Framebuffer::Create(specs);

	m_SceneViewPanel = Olala::CreateRef<SceneViewPanel>(editorCamera);
	m_PropertyPanel = Olala::CreateRef<PropertyPanel>();
	m_SceneHierarchyPanel = Olala::CreateRef<SceneHierarchyPanel>(m_Scene, m_PropertyPanel);
    m_RuntimeViewPanel = Olala::CreateRef<RuntimeViewPanel>();

	Olala::Ref<Olala::Texture2D> exampleTextures[2];
	exampleTextures[0] = Olala::Texture2D::Create("../Olala/Asset/Planet9.jpg");
	exampleTextures[1] = Olala::Texture2D::Create("../Olala/Asset/Abe.png");

	// Quads
	for (int i = 0; i < 8; i++)
	{
		Olala::Entity quad = m_Scene->CreateEntity("Quad" + std::to_string(i));
		quad.AddComponent<Olala::SpriteRendererComponent>(glm::vec2{ 80.f, 45.f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, exampleTextures[i%2]);
		quad.GetComponent<Olala::TransformComponent>().Position.x = -350.f + 100.f * i;
	}

}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float dt)
{
    //Moved to OnImGuiRender becuase of framebuffer resizing
	//m_Scene->OnUpdate();
}

void EditorLayer::OnImGuiRender()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // Menu bar
    DrawMenuBar();

    // Panels
    m_SceneHierarchyPanel  -> OnImGuiRender();
    m_PropertyPanel        -> OnImGuiRender();
    m_SceneViewPanel       -> OnImGuiRender();
    m_RuntimeViewPanel     -> OnImGuiRender();

    // On update must be because sceneViewPanel resizes framebuffer
    m_Scene->OnUpdate();


    ImGui::End();

    ImGui::ShowDemoWindow();
}

void EditorLayer::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{

			}
			if (ImGui::MenuItem("Save"))
			{

			}
            ImGui::Separator();
            if (ImGui::MenuItem("Close"))
            {
                Olala::Application::Get().Close();
            }
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Panel"))
            {
                if (ImGui::MenuItem("Entities"))
                {
                    m_SceneHierarchyPanel->SetOpen(true);
                }
                if (ImGui::MenuItem("Scene"))
                {
                    m_SceneViewPanel->SetOpen(true);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
		ImGui::EndMenuBar();
	}
    
}

void EditorLayer::OnEvent(Olala::Event& e)
{
}
