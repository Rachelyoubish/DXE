#pragma once


#ifdef DXE_PLATFORM_WINDOWS

extern DXE::Application* DXE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = DXE::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif