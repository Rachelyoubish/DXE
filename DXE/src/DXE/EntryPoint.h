#pragma once


#ifdef DXE_PLATFORM_WINDOWS

extern DXE::Application* DXE::CreateApplication();

int CALLBACK WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	// Initialize console first, therefore all of spdlogs can be sent to it. 
	AllocConsole();
	FILE* file = nullptr;
	freopen_s( &file, "CONIN$", "r", stdin );
	freopen_s( &file, "CONOUT$", "w", stdout );

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