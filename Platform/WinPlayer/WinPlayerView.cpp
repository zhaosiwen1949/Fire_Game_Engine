#include "WinPlayerView.h"
#include "Runtime/IO/FileSystem.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Scene/SceneManager.h"
#include <algorithm>

extern "C" void Render();
extern "C" void InitRuntime();
static WinPlayerView* sWinPlayerView = nullptr;
static bool sPermenantDisable = false;
extern int WinPlayerViewWidth;
extern int WinPlayerViewHeight;
static int sFT=26, sFB=4, sFL=3, sFR=3;
WinPlayerView*GetWinPlayerView()
{
	return sWinPlayerView;
}
void WinPlayerView::Init(HWND parent)
{
	sWinPlayerView = this;
	mbRightButtonDown = false;
	mbLeftButtonDown = false;
	RegisterWindowClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, L"GLWindow", WindowEventProc);
	mhWnd = CreateWindowEx(NULL, L"GLWindow", L"GLRenderWindow", WS_CHILD, 0, 0, 32, 32, parent, nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
	mHDC = ::GetDC(mhWnd);
	int xDPI = GetDeviceCaps(mHDC, LOGPIXELSX);//每英寸逻辑像素数 水平
	int yDPI = GetDeviceCaps(mHDC, LOGPIXELSY);
	SetupSimpleGLContext();
	glewInit();
	MakeCurrent();
	GetSceneManager()->OnViewSizeChange(0, 0, 1280, 720);
	InitRuntime();
	DragAcceptFiles(mhWnd, TRUE);
}

void WinPlayerView::PaintWindow(){
	mbNeedToUpdateGLWindow = true;
	Render();
}

POINT WinPlayerView::mCursorPos;
bool sbEnableEvent = true;
void WinPlayerView::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	GLWindow::OnLButtonDown(wParam, lParam, reserved);
	mbLeftButtonDown = true;
	SetCapture(mhWnd);
	if (sbEnableEvent)
	{

	}
}

void WinPlayerView::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
	GLWindow::OnSize(wParam, lParam, reserved);
}

void WinPlayerView::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	GLWindow::OnLButtonUp(wParam, lParam, reserved);
	mbLeftButtonDown = false;
	if (mbRightButtonDown == false && mbLeftButtonDown == false) {
		ReleaseCapture();
	}
	if (sbEnableEvent)
	{

	}
}

void WinPlayerView::OnRButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	GLWindow::OnRButtonDown(wParam, lParam, reserved);
	mbRightButtonDown = true;
	SetCapture(mhWnd);
	if (sbEnableEvent)
	{

	}
}
void WinPlayerView::OnRButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	GLWindow::OnRButtonUp(wParam, lParam, reserved);
	mbRightButtonDown = false;
	if (mbRightButtonDown == false && mbLeftButtonDown == false) {
		ReleaseCapture();
	}
	if (sbEnableEvent)
	{

	}
}
void WinPlayerView::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
{
	GLWindow::OnMouseMove(wParam, lParam, reserved);
	mCursorPos.x = LOWORD(lParam)>10000 ?0: LOWORD(lParam);
	mCursorPos.y = HIWORD(lParam)>10000 ?0:HIWORD(lParam);
	if (sbEnableEvent)
	{

	}
}

void WinPlayerView::OnDropFiles(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	UINT  nFileCount = ::DragQueryFile((HDROP)wParam, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH];
	DWORD dwAttribute;
	for (UINT i = 0; i < nFileCount; i++){
		memset(szFileName, 0, _MAX_PATH);
		::DragQueryFile((HDROP)wParam, i, szFileName, _MAX_PATH);
		dwAttribute = ::GetFileAttributes(szFileName);
		if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY){

		}else{

		}
	}
}