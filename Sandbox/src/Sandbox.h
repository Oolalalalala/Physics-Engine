#pragma once

#include <Olala.h>

class Sandbox : public Olala::Application
{
public:
	Sandbox();
	~Sandbox();
};

Olala::Application* Olala::CreateApplication()
{
	return new Sandbox();
}