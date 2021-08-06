#include "Platform/Win32Common/RenderWindow.h"

class WinPlayerView :public Editor::GLWindow
{
protected:
	void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnRButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnRButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnSize(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */);
	void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnDropFiles(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
public:
	bool mbRightButtonDown;
	bool mbLeftButtonDown;
	POINT mOriginalPos;
	void Init(HWND parent);
	virtual void PaintWindow();
public:
	static POINT mCursorPos;
};
WinPlayerView*GetWinPlayerView();