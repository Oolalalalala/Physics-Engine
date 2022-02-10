#pragma once

// Use box2d library
#define USE_BOX2D

#include <Windows.h>


#include "Core/Application.h"
#include "Core/Base.h"
#include "Core/Log.h"
#include "Core/Layer.h"
#include "Core/IO.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

// Renderer
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Shader.h"
#include "Renderer/Framebuffer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"



#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"
#include "Scene/AssetManager.h"
#include "Scene/AssetPool.h"
#include "Scene/SceneSerializer.h"


#include "Utils/FileDialog.h"


#include "glm/glm.hpp"
#include "imgui/imgui.h"