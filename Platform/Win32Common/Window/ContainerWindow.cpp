#include "ContainerWindow.h"
#include "Runtime/Debugger/Log.h"
namespace Editor {
	ContainerWindow::ContainerWindow() {
		mbDraging = false;
		mMarginLeft = 3;
		mMarginRight = 3;
		mMarginTop = 26;
		mMarginBottom = 4;
	}
	void ContainerWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect.X = rect.left;
		mRect.Y = rect.top;
		mRect.Width = rect.right - rect.left;
		mRect.Height = rect.bottom - rect.top;
	}
	void ContainerWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		SetCapture(mhWnd);
		mbDraging = true;
		POINT pos;
		GetCursorPos(&pos);
		RECT windowRect;
		GetWindowRect(mhWnd, &windowRect);
		mDeltaWhenDrag.X = pos.x - windowRect.left;
		mDeltaWhenDrag.Y = pos.y - windowRect.top;
	}
	void ContainerWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ReleaseCapture();
		mbDraging = false;
	}
	void ContainerWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (mbDraging)
		{
			POINT pos;
			GetCursorPos(&pos);
			//Debug("%d %d %d %d", pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
			MoveWindow(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
		}
		DoubleBufferedWindow::OnMouseMove(wParam, lParam, reserved);
	}

	void ContainerWindow::Init(){
		DWORD windowStyle = WS_OVERLAPPED   | WS_CLIPCHILDREN;
		mhWnd = CreateWindowEx(NULL, L"ContainerWindow", NULL,
			windowStyle, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, GetModuleHandle(NULL), nullptr);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
	}

	LRESULT ContainerWindow::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		//OnNCPAINT(hWnd, message, wParam, lParam, reserved);
		InvalidateRect(hWnd, NULL, false);
		return 0;
	}

	LRESULT ContainerWindow::OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		NCCALCSIZE_PARAMS* pParams = NULL;
		RECT* pRect = NULL;
		 
		BOOL bValue = static_cast<BOOL>(wParam);
		if (bValue) pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
		else pRect = reinterpret_cast<RECT*>(lParam);

		if (bValue)
			pRect = &pParams->rgrc[0];

		if (bValue) {
			pRect->left = pRect->left + mMarginLeft;
			pRect->top = pRect->top + mMarginTop;
			pRect->right = pRect->right - mMarginRight;
			pRect->bottom = pRect->bottom - mMarginBottom;
			pParams->rgrc[1] = pParams->rgrc[0];
		}
		else DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}

	LRESULT ContainerWindow::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		HDC WindowDC = GetWindowDC(hWnd);
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		OffsetRect(&ClientRect, mMarginLeft, mMarginTop);
		ExcludeClipRect(WindowDC, ClientRect.left, ClientRect.top,
			ClientRect.right, ClientRect.bottom);
		Gdiplus::Graphics painter(WindowDC);
		painter.Clear(mBKGColor);
		ReleaseDC(hWnd, WindowDC);
		InvalidateRect(mhWnd, nullptr, false);
		return 0;
	}
	LRESULT ContainerWindow::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT WindowRect;
		GetWindowRect(hWnd, &WindowRect);
		POINT point;
		point.x = LOWORD(lParam) - WindowRect.left;
		point.y = HIWORD(lParam) - WindowRect.top;
		Rect rect = { 0, 3, 3, mRect.Height - 6 };
		if (rect.Contains(point.x,point.y))
			return HTLEFT;

		rect = { mRect.Width - 3, 3, mRect.Width, mRect.Height - 6 };
		if (rect.Contains(point.x, point.y))
			return HTRIGHT;

		rect = { 3, mRect.Height - 4, mRect.Width - 6, 4 };
		if (rect.Contains(point.x, point.y))
			return HTBOTTOM;

		rect = { 3, 0, mRect.Width - 6, 3 };
		if (rect.Contains(point.x, point.y))
			return HTTOP;

		return HTCLIENT;
	}

	void ContainerWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW | CS_DBLCLKS , L"ContainerWindow", WindowEventProc);
	}
}