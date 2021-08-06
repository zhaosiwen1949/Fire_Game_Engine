#include "WinPlayerMainWindow.h"
#include "WinPlayerView.h"
#include <dwmapi.h>
#include "Runtime/Debugger/Log.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#pragma comment(lib,"dwmapi.lib")
extern bool sbEnableEvent;
extern WinPlayerView *cv;
void WinPlayerMainWindow::DrawContent(Gdiplus::Graphics&painter)
{
	//mAliceIcon->Draw(painter);
	if(mCloseBtn!=nullptr)mCloseBtn->Draw(painter);
	mTitle->Draw(painter);
}

void WinPlayerMainWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	if (sbEnableEvent){
		int fwKeys = GET_KEYSTATE_WPARAM(wParam);
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	}
}

void WinPlayerMainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
	MainWindow::OnSize(wParam, lParam, reserved);
	if(mCloseBtn!=nullptr)mCloseBtn->SetRect(mRect.Width-32, 2, 24, 24);
	mTitle->SetRect(0, 6, mRect.Width, 30);
}

void WinPlayerMainWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	if (mCloseBtn!=nullptr&&mCloseBtn->OnClickDown(x,y)){
		InvalidateRect(mhWnd, NULL, true);
		SetCapture(mhWnd);
	}else{
		MainWindow::OnLButtonDown(wParam, lParam, reserved);
	}
}

void WinPlayerMainWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	if (mCloseBtn!=nullptr&&mCloseBtn->IsDown()) {
		mCloseBtn->OnClickUp(x, y);
		mCloseBtn->ResetState();
		ReleaseCapture();
	}else {
		MainWindow::OnLButtonUp(wParam, lParam, reserved);
	}
	InvalidateRect(mhWnd, NULL, true);
}

void WinPlayerMainWindow::OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
	if (sbEnableEvent&&!mbIsIMEInputMode) {
		int keyCode = wParam;
	}
}

void WinPlayerMainWindow::OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
	if (sbEnableEvent&&!mbIsIMEInputMode) {
		int keyCode = wParam;
	}
}
void WinPlayerMainWindow::OnChar(WPARAM wParam, LPARAM lParam, void*reserved)
{
	if (sbEnableEvent)
	{

	}
}
void WinPlayerMainWindow::OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved)
{
	if (sbEnableEvent)
	{

	}
}
void WinPlayerMainWindow::OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {

}
void WinPlayerMainWindow::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	UINT nNotify = HIWORD(wParam);
	UINT commandID = LOWORD(wParam);
	switch (commandID)
	{
	case 1:
		{
			if (!OpenClipboard(mhWnd))
				return;
			HGLOBAL   hglb;
			hglb = GetClipboardData(CF_UNICODETEXT);
			if (hglb != NULL)
			{
				LPTSTR    lptstr= (LPTSTR)GlobalLock(hglb);
				if (lptstr != NULL)
				{
					Debug("on paste");
					GlobalUnlock(hglb);
				}
			}
			EmptyClipboard();
			CloseClipboard();
		}
		break;
	}
}

void WinPlayerMainWindow::Init()
{
	SetRootWindow(this);
	RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW  | CS_DBLCLKS, L"WinPlayerMainWindow", WindowEventProc);
	DWORD windowStyle = WS_OVERLAPPED |WS_CLIPCHILDREN |WS_CLIPSIBLINGS ;
	mhWnd = CreateWindowEx(NULL, L"WinPlayerMainWindow", L"WinPlayer", windowStyle, 0, 0, 32, 32, NULL, NULL, mAppInstance, NULL);
	SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
	mHDC = GetWindowDC(mhWnd);
	SetRect(0, 0, 32, 32);
	mBKGColor = Color(0, 150, 200);
	mAliceIcon = nullptr;
	mCloseBtn = nullptr;
	if (Alice::FileSystem::Exists("CloseButton.png")) {
		mCloseBtn = new Editor::ImageButton;
		mCloseBtn->SetImagePath("CloseButton.png");
		mCloseBtn->SetRect(1248, 2, 24, 24);
		mCloseBtn->SetOnClick([]()->void {Debug("quit"); PostQuitMessage(0); });
	}
	mTitle = new Editor::StaticText;
	mTitle->SetAligning(Editor::AligningModeMiddle);
	mTitle->SetRect(0, 10, 1280,30);
	mTitle->SetText("Hello World");
	ACCEL accel[] =
	{
		{ FCONTROL | FVIRTKEY, 'V', 1 }//,
		//{ FCONTROL | FVIRTKEY, 'C', 2 }
	};
	mAccel = CreateAcceleratorTable(accel, 1);// 2);
}

void WinPlayerMainWindow::UpdateTitle() {
	const char *renderer = (const char*)glGetString(GL_RENDERER);
	const char *glVersion = (const char*)glGetString(GL_VERSION);
	char szTitle[128] = { 0 };
	strcpy(szTitle, "GPU : ");
	strcat(szTitle, renderer);
	strcat(szTitle, " ,OpenGL Version : ");
	strcat(szTitle, glVersion);
	mTitle->SetText(szTitle);
}
void WinPlayerMainWindow::SetWindowSize(int marginL, int marginR, int marginTop, int marginB, int child_width, int child_height, bool fullscreen_mode) {
	HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
	int clientWidth;
	int clientHeight;
	if (monitor) {
		MONITORINFO info;
		info.cbSize = sizeof(info);
		GetMonitorInfo(monitor, &info);
		clientWidth = info.rcWork.right - info.rcWork.left;
		clientHeight = info.rcWork.bottom - info.rcWork.top;
	}
	int x = clientWidth / 2 - child_width / 2 - mMarginLeft;
	int y = clientHeight / 2 - (child_height + mMarginTop + mMarginBottom) / 2;
	int width = child_width + mMarginLeft + mMarginRight;
	int height = child_height + mMarginTop + mMarginBottom;
	MoveWindow(x, y, width, height);
	SetWindowPos(mhWnd, HWND_NOTOPMOST, x, y, width, height, SWP_SHOWWINDOW);
}
LRESULT WinPlayerMainWindow::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	return HTCLIENT;
}