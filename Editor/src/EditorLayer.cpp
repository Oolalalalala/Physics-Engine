#include "EditorLayer.h"


void EditorLayer::OnAttach()
{
	m_Scene = Olala::CreateRef<Olala::Scene>();
    m_SceneSerializer = Olala::CreateRef<Olala::SceneSerializer>(m_Scene);
    m_SceneSerializer->Deserialize("../Olala/Asset/Scene/Demo/TestScene.Olala");

    // Editor Camera
    auto cameraView = m_Scene->GetAllEntitiesWith<Olala::TagComponent, Olala::CameraComponent>();
    for (auto e : cameraView)
    {
        if (cameraView.get<Olala::TagComponent>(e).Tag == "Editor Camera") 
            m_EditorCamera = { e, m_Scene.get() };
    }
    m_EditorCamera.AddComponent<Olala::EditorCameraControllerComponent>();


    // Panels
	m_SceneViewPanel = Olala::CreateRef<SceneViewPanel>(m_Scene, m_EditorCamera);
	m_PropertyPanel = Olala::CreateRef<PropertyPanel>();
	m_SceneHierarchyPanel = Olala::CreateRef<SceneHierarchyPanel>(m_Scene, m_PropertyPanel);
    m_RuntimeViewPanel = Olala::CreateRef<RuntimeViewPanel>();
    m_AssetPanel = Olala::CreateRef<AssetPanel>(m_Scene->GetAssetManager());
    m_DebugPanel = Olala::CreateRef<DebugPanel>(m_SceneViewPanel);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float dt)
{
    if (m_EditorCamera.HasComponent<Olala::EditorCameraControllerComponent>())
        m_EditorCamera.GetComponent<Olala::EditorCameraControllerComponent>().IsOn = m_SceneViewPanel->GetIsFocused();

    if (m_IsRuntime)
        m_RuntimeScene->OnRuntimeUpdate(dt);
    else
        m_Scene->OnUpdate(dt);


    m_SceneHierarchyPanel   ->  OnUpdate(dt);
    m_PropertyPanel         ->  OnUpdate(dt);
    m_SceneViewPanel        ->  OnUpdate(dt);
    m_RuntimeViewPanel      ->  OnUpdate(dt);
    m_AssetPanel            ->  OnUpdate(dt);
    m_DebugPanel            ->  OnUpdate(dt);
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
    m_AssetPanel           -> OnImGuiRender();
    m_DebugPanel           -> OnImGuiRender();


    ImGui::End();

    //ImGui::ShowDemoWindow();
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
            if (ImGui::MenuItem("Entities"))   m_SceneHierarchyPanel->SetOpen(true);
            if (ImGui::MenuItem("Scene"))      m_SceneViewPanel->SetOpen(true);
            if (ImGui::MenuItem("Property"))   m_PropertyPanel->SetOpen(true);
            if (ImGui::MenuItem("Asset"))      m_AssetPanel->SetOpen(true);
            if (ImGui::MenuItem("Debug"))      m_DebugPanel->SetOpen(true);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Program"))
        {
            if (ImGui::MenuItem(m_IsRuntime ? "Stop" : "Start", "F5"))
            {
                m_IsRuntime = !m_IsRuntime;
                if (m_IsRuntime)
                    OnRuntimeBegin();
                else
                    OnRuntimeEnd();
            }
            if (ImGui::MenuItem("Pause"))
            {
                m_IsPausing = true;
            }
            ImGui::EndMenu();
        }

		ImGui::EndMenuBar();
	}
    
}

void EditorLayer::OnRuntimeBegin()
{
    m_RuntimeScene = Olala::Scene::Copy(m_Scene);
    m_RuntimeScene->InitializePhysics();

    m_SceneViewPanel->SetScene(m_RuntimeScene);
    m_SceneHierarchyPanel->SetDisplayingScene(m_RuntimeScene);

    auto cameraView = m_RuntimeScene->GetAllEntitiesWith<Olala::TagComponent, Olala::CameraComponent>();
    for (auto e : cameraView)
    {
        if (cameraView.get<Olala::TagComponent>(e).Tag == "Editor Camera")
        {
            m_SceneViewPanel->SetCamera(Olala::Entity(e, m_RuntimeScene.get()));
            break;
        }
    }

    m_PropertyPanel->SetDisplayedEntity(Olala::Entity());
    m_PropertyPanel->SetIsRuntime(true);
}

void EditorLayer::OnRuntimeEnd()
{
    m_SceneViewPanel->SetScene(m_Scene);
    m_SceneHierarchyPanel->SetDisplayingScene(m_Scene);

    auto cameraView = m_Scene->GetAllEntitiesWith<Olala::TagComponent, Olala::CameraComponent>();
    for (auto e : cameraView)
    {
        if (cameraView.get<Olala::TagComponent>(e).Tag == "Editor Camera")
        {
            m_SceneViewPanel->SetCamera(Olala::Entity(e, m_Scene.get()));
            break;
        }
    }

    m_PropertyPanel->SetDisplayedEntity(Olala::Entity());
    m_PropertyPanel->SetIsRuntime(false);

    m_RuntimeScene = nullptr;
}

void EditorLayer::OnEvent(Olala::Event& e)
{
}
