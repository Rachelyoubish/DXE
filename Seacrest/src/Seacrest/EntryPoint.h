#pragma once


#ifdef SEACREST_PLATFORM_WINDOWS

extern Seacrest::Application* Seacrest::CreateApplication();

// The entry point for our Windows application
int WINAPI WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	// Allocate a new console for calling. 
	// Initialize console first, therefore 
	// all of spdlog can be sent to it. 
	// AllocConsole();
	// FILE* file = nullptr;
	// freopen_s( &file, "CONIN$", "r", stdin );
	// freopen_s( &file, "CONOUT$", "w", stdout );

	// Dear ImGui now handles the logger's output. 

	Seacrest::Log::Init();
	SEACREST_CORE_WARN( "Initialized Log!" );
	int a = { 5 };
	SEACREST_INFO( "Hello! Var={0}", a );

	auto app = Seacrest::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif