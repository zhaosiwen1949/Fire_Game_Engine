#include "Platform/Win32Common/MainWindow.h"
#include "Platform/Win32Common/Control/ImageControl/ImageControl.h"
#include "Platform/Win32Common/Control/ButtonControl/ImageButton.h"
#include "Platform/Win32Common/Control/ImageControl/SelectableImageControl.h"
#include "Platform/Win32Common/Control/TextControl/StaticText.h"

class WinPlayerMainWindow :public Editor::MainWindow
{
protected:
	Editor::ImageControl*mAliceIcon, *mCourseText;
protected:
	void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void DrawContent(Gdiplus::Graphics&painter);
	void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
public:
	Editor::StaticText*mTitle;
	Editor::ImageButton*mCloseBtn;
	HACCEL mAccel;
	void Init();
	void SetWindowSize(int marginL, int marginR, int marginTop, int marginB,int child_width,int child_height,bool fullscreen_mode);
	void UpdateTitle();
};