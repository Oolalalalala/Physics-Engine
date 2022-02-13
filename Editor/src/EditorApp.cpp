#include <Olala.h>
#include <Core/EntryPoint.h>

#include "EditorLayer.h"

class Editor : public Olala::Application
{
public:
	Editor()
		: Application(Olala::ApplicationSpecs{
			"Editor",                                                    // Name
			1280, 720,                                                   // Size
			"../Olala/Asset/Internal/Texture/small_icon.png",            // Icon
			"",                                                          // Small Icon
			true                                                         // VSync
		  })
	{
		PushLayer(new EditorLayer());
	}

	~Editor()
	{

	}
};

Olala::Application* Olala::CreateApplication()
{
	return new Editor();
}
