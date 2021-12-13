#pragma once
#include "Application.h"

extern Olala::Application* Olala::CreateApplication();

int main(int argc, char* argv[])
{
	auto* app = Olala::CreateApplication();
	
	app->Run();
	
	delete app;
}