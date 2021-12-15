#include "pch.h"
#include "ImGuiLayer.h"

#include "Core/Application.h"
#include "Renderer/RenderCommand.h"

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Olala {

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void ImGuiLayer::OnUpdate(float dt)
	{
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
	}

}