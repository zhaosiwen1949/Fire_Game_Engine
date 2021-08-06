#include "Project/NewProjectContainerWindow.h"
#include "Runtime/Utils/Timer.h"
#include "Runtime/Debugger/Log.h"
#pragma comment(lib,"shlwapi.lib")
Editor::NewProjectContainerWindow wnd;
extern "C" void InitEngineCore();
extern "C" void OnQuitEngine();
#if _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

void ProcessingMessage(MSG&msg) {
	if (!TranslateAccelerator(msg.hwnd, wnd.mAccel, &msg)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void MessageLoop() {
	MSG msg;
	bool isQuit = false;
	Alice::Timer t;
	std::vector<MSG> msgs;
	int nIndex = 0;
	int nMsgCount = 0;
	while (!isQuit) {
		t.Reset();
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				ProcessingMessage(msg);
			}
			else {
				msgs.push_back(msg);
				if (msgs.size() == 100) {
					break;
				}
			}
		}
		nIndex = 0;
		nMsgCount = (int)msgs.size();
		while (nIndex < nMsgCount) {
			if (WM_QUIT == msgs[nIndex].message)
			{
				isQuit = true;
				Debug("receive quit message");
				OnQuitEngine();
			}
			else
			{
				ProcessingMessage(msgs[nIndex]);
			}
			nIndex++;
		}
		msgs.clear();
		int elapsedTime = (int)t.GetTimeInMsi();
		static const int maxTimePerFrame = 16;
		int sleepTime = maxTimePerFrame - elapsedTime;
		if (sleepTime > 0) {
			::Sleep((DWORD)sleepTime);
		}
	}
}
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Editor::NewProjectContainerWindow::InitGlobalGUISettings(hInstance);
	InitEngineCore();
	wnd.Init();
	wnd.Show(true);
	wnd.MoveWindow(100, 100, 1280,720);
	MessageLoop();
	return 0;
}