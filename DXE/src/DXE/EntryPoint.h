#pragma once


#ifdef DXE_PLATFORM_WINDOWS

extern DXE::Application* DXE::CreateApplication();

int main(int argc, char** argv)
{
	DXE::Log::Init();
	DXE_CORE_WARN( "Initialized Log!" );
	int a = { 5 };
	DXE_INFO( "Hello! Var={0}", a );

	auto app = DXE::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif