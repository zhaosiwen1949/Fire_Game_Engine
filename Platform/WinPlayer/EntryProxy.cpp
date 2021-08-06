#include "WinPlayerMainWindow.h"
#include "WinPlayerView.h"
#include "Runtime/LuaEngine/LuaEngine.h"
WinPlayerMainWindow gMainWindow;
WinPlayerView *cv;
int WinPlayerViewWidth = 1280;
int WinPlayerViewHeight = 720;
extern "C" void InitEngineCore();
extern "C" void OnQuitEngine();
void ProcessingMessage(MSG&msg) {
	if (!TranslateAccelerator(msg.hwnd, gMainWindow.mAccel, &msg)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void MessageLoop() {
	MSG msg;
	bool isQuit = false;
	std::vector<MSG> msgs;
	int nIndex = 0;
	int nMsgCount = 0;
	while (!isQuit) {
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
				OnQuitEngine();
				break;
			}
			else
			{
				ProcessingMessage(msgs[nIndex]);
			}
			nIndex++;
		}
		msgs.clear();
		if (false==isQuit) {
			cv->PaintWindow();
			Editor::GLWindow::Present();
		}
	}
}
extern "C" void AliceGo(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	WinPlayerMainWindow::InitGlobalGUISettings(hInstance);
	InitEngineCore();
	gMainWindow.Init();
	gMainWindow.Show(true);
	gMainWindow.SetWindowSize(3, 3, 26, 4, WinPlayerViewWidth, WinPlayerViewHeight, false);
	cv = new WinPlayerView;
	cv->Init(gMainWindow.GetHwnd());
	cv->MoveWindow(0, 0, WinPlayerViewWidth, WinPlayerViewHeight);
	gMainWindow.UpdateTitle();
	Alice::LuaEngine::StartFromEntry();
	cv->Show(true);
	MessageLoop();
}