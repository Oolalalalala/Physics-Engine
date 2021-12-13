#include <Olala.h>
#include <Core/EntryPoint.h>

#include "EditorLayer.h"

class Editor : public Olala::Application
{
public:
	Editor()
		: Application("Olala Editor")
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
