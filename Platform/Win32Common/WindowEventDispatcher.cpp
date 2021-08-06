#include "WindowEventDispatcher.h"
#include "Runtime/Debugger/log.h"
#pragma comment(lib,"imm32.lib")
namespace Editor{
	WindowEventDispatcher::WindowEventDispatcher() :mbMouseLeaved(true)
	{
		mbIsIMEInputMode = false;
		mbIsIMEInputModeDelay = false;
	}

	void WindowEventDispatcher::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnLButtonDoubleClick(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void WindowEventDispatcher::OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void WindowEventDispatcher::OnChar(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void WindowEventDispatcher::OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void WindowEventDispatcher::OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {

	}
	void WindowEventDispatcher::OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void WindowEventDispatcher::OnCloseWindow(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnFocus(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnSize(WPARAM wParam, LPARAM lParam, void*reserved)
	{

	}

	void WindowEventDispatcher::OnMove(WPARAM wParam, LPARAM lParam, void*reserved)
	{
	}

	void WindowEventDispatcher::OnMoving(WPARAM wParam, LPARAM lParam, void*reserved)
	{
	}

	void WindowEventDispatcher::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnPaint() {
	}

	void WindowEventDispatcher::OnTimer(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}


	void WindowEventDispatcher::OnRButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnRButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnMiddleButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnMiddleButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnDropFiles(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void WindowEventDispatcher::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (mbMouseLeaved)
		{
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE | TME_HOVER;
			csTME.hwndTrack = mhWnd;
			csTME.dwHoverTime = 10;
			TrackMouseEvent(&csTME);
			mbMouseLeaved = false;
		}
	}

	void WindowEventDispatcher::OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		mbMouseLeaved = true;
	}

	LRESULT WindowEventDispatcher::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCLBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCLBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCLBUTTONDBLCLK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCMOUSEMOVE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCRBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::OnNCRBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowEventDispatcher::WindowEventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowEventDispatcher*self = (WindowEventDispatcher*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (self == nullptr)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		switch (message)
		{
		case WM_MOUSEMOVE:
		{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnMouseMove(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
		return 0;
		case WM_MOUSELEAVE:
			self->OnMouseLeave(wParam, lParam);
			return 0;
		case WM_LBUTTONDOWN:
		{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnLButtonDown(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
		return 0;
		case WM_LBUTTONUP:
		{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnLButtonUp(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
		return 0;
		case WM_LBUTTONDBLCLK:
			self->OnLButtonDoubleClick(wParam, lParam);
			return 0;
		case WM_RBUTTONDOWN:
			self->OnRButtonDown(wParam, lParam);
			return 0;
		case WM_RBUTTONUP:
			self->OnRButtonUp(wParam, lParam);
			return 0;
		case WM_MBUTTONDOWN:
			self->OnMiddleButtonDown(wParam, lParam);
			return 0;
		case WM_MBUTTONUP:
			self->OnMiddleButtonUp(wParam, lParam);
			return 0;
		case WM_MOUSEWHEEL:
			self->OnMouseWheel(wParam, lParam);
			return 0;
		case WM_KEYDOWN:
			if (false == self->mbIsIMEInputMode) {
				//Debug("WM_KEYDOWN");
				self->OnKeyDown(wParam, lParam);
			}
			return 0;
		case WM_KEYUP:
			if (false == self->mbIsIMEInputMode) {
				//Debug("WM_KEYUP");
				self->OnKeyUp(wParam, lParam);
			}
			if (self->mbIsIMEInputModeDelay==false){
				self->mbIsIMEInputMode = false;
			}
			return 0;
		case WM_CHAR:
			if (false==self->mbIsIMEInputMode){
				//Debug("WM_CHAR");
				self->OnChar(wParam, lParam);
			}
			return 0;
		case WM_IME_STARTCOMPOSITION:
			//Debug("WM_IME_STARTCOMPOSITION");
			self->mbIsIMEInputMode = true;
			self->mbIsIMEInputModeDelay = true;
			break;
		case WM_IME_ENDCOMPOSITION:
			//Debug("WM_IME_ENDCOMPOSITION");
			self->mbIsIMEInputModeDelay = false;
			break;
		case WM_IME_COMPOSITION://set marked text/set ime text
		{
			//Debug("WM_IME_COMPOSITION %d", lParam);
			self->OnCompositionIMEString(wParam, lParam);
		}
			break;
		case WM_IME_CHAR:
			self->OnIMEChar(wParam, lParam);
			break;
		case WM_COMMAND:
			self->OnCommand(wParam, lParam);
			return 0;
		case WM_DROPFILES:
			self->OnDropFiles(wParam, lParam);
			return 0;
		case WM_CLOSE:
			self->OnCloseWindow(wParam, lParam);
			return 0;
		case WM_SETFOCUS:
			self->OnFocus(wParam, lParam);
			return 0;
		case WM_ACTIVATE:
			return 0;
		case WM_ACTIVATEAPP:
			return 0;
		case WM_SIZE:
			self->OnSize(wParam, lParam);
			return 0;
		case WM_MOVE:
			self->OnMove(wParam, lParam);
			return 0;
		case WM_MOVING:
			self->OnMoving(wParam, lParam);
			return TRUE;
		case WM_GETMINMAXINFO:
			self->OnGetMinMaxInfo(wParam, lParam);
			return 0;
		case WM_TIMER:
			self->OnTimer(wParam, lParam);
			return 0;
		case WM_ERASEBKGND:

			return 0;
		case WM_PAINT://
		{
			RECT rect;
			if (FALSE != GetUpdateRect(hWnd, &rect, true)) {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				self->OnPaint();
				EndPaint(hWnd, &ps);
			}
		}
		return 0;
		case WM_NCPAINT:
			self->OnNCPAINT(hWnd, message, wParam, lParam);
			return 0;
		case WM_NCACTIVATE:
			self->OnNCACTIVATE(hWnd, message, wParam, lParam);
			if (FALSE==wParam){
				return TRUE;
			}
			return FALSE;//this will be ignored
		case WM_NCCALCSIZE:
			return self->OnNCCALCSIZE(hWnd, message, wParam, lParam);
		case WM_NCHITTEST:
			return self->OnNCHITTEST(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONUP:
			return self->OnNCLBUTTONUP(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONDOWN:
			return self->OnNCLBUTTONDOWN(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONDBLCLK:
			return self->OnNCLBUTTONDBLCLK(hWnd, message, wParam, lParam);
		case WM_NCMOUSEMOVE:
			return self->OnNCMOUSEMOVE(hWnd, message, wParam, lParam);
		case WM_NCRBUTTONUP:
			return self->OnNCRBUTTONUP(hWnd, message, wParam, lParam);
		case WM_NCRBUTTONDOWN:
			return self->OnNCRBUTTONDOWN(hWnd, message, wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}