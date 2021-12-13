#include <Olala.h>
#include <Core/EntryPoint.h>

class Sandbox : public Olala::Application
{
public:
	Sandbox()
		: Application("Sandbox")
	{

	}
	~Sandbox()
	{

	}
};

Olala::Application* Olala::CreateApplication()
{
	return new Sandbox();
}