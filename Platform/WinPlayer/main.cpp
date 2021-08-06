#include <windows.h>
#if _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif
extern "C" void AliceGo(HINSTANCE, HINSTANCE, LPSTR, int);
extern "C" void SetCoreVersion(int version);
extern "C" void SetEntryScript(const char * entry_script_bundle, const char * init_script);
INT WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	SetEntryScript(nullptr, "init");
	SetCoreVersion(1);
	AliceGo(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	return 0;
}