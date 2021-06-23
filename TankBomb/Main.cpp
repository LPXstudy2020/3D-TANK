#include "GameApp.h"
 


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
				   _In_ LPSTR cmdLine, _In_ int showCmd)
{
	// 这些参数不使用
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//开启控制台
#ifdef _DEBUG
	AllocConsole();
	SetConsoleTitleA("Debug Win output");
	FILE* consoleContent;
	freopen_s(&consoleContent, "CONOUT$", "w", stdout);
	freopen_s(&consoleContent, "CONOUT$", "w", stderr);
	freopen_s(&consoleContent, "CONIN$", "r", stdin);
	//HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

#endif
	printf("oh~~~~");

	GameApp theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}




