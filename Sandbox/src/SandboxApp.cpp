#include <DXE.h>

class Sandbox : public DXE::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

DXE::Application* DXE::CreateApplication()
{
	return new Sandbox();
}