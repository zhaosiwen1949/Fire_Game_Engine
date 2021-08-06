#pragma once
#include "WindowEventDispatcher.h"
namespace Editor{
	enum WindowStyle{
		WindowStyleNormalWindow=1,
		WindowStyleDlgWindow,
		WindowStyleContainerWindow,
		WindowStyleAuxWindow,
		WindowStyleCount
	};
	class SplitLine;
	enum SplitLinePos;
	class BaseWindow:public WindowEventDispatcher
	{
	protected:
		HDC mHDC;
		BaseWindow*mParent;
		Color mBKGColor;
		Rect mRect,mMinRect,mMaxRect;
		char mName[64];
		char mType[64];
		bool mbDirty;
		bool mbVisiable;
		bool mbCloseForMaximize;
		bool mbHasMiniSize;
		SplitLine*mLeftSplitLine, *mRightSplitLine, *mUpperSplitLine, *mBottomSplitLine;
		std::unordered_set<BaseWindow*> mWindowMoveEventListeners;
		std::unordered_set<BaseWindow*> mChildren;
	protected:
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void OnPaint();
		virtual void OnEndPaint();
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		BaseWindow();
		virtual ~BaseWindow();
		virtual void OnWindowMove(BaseWindow*movedWindow);
		virtual void OnEditControlLoseFocus(BaseWindow*editControl);

		void SetMinRect(int x, int y, int width, int height);
		void SetBkgColor(Color &color);
		void SetParent(BaseWindow*parent);
		void SetHWND_DC(HWND hwnd, HDC hdc);
		void SetRect(int x, int y, int width, int height);
		void SetRect(Rect &rect);
		void SetSize(int width, int height, HWND param);
		void SetWindowName(const char*name);
		void SetType(const char* typeName);
		SplitLine*SetSplitLine(SplitLinePos pos, SplitLine*splitLine);

		Rect &GetMinRect();
		int GetMinWidth();
		int GetMinHeight();

		void ScheduleUpdate();
		void CancelUpdate();
		void Update();
		virtual void PaintWindow();
		int GetWidth();
		int GetHeight();
		Rect GetRect();
		virtual void MoveWindow(int x, int y, int width, int height);
		int GetX();
		int GetY();
		BaseWindow*GetParent();
		HWND GetHwnd();
		const char*GetWindowName();
		virtual void Show(bool bShow);
		virtual void ShowOnTop();
		virtual void OnShow(bool bShow);
		HDC GetDC();
		const char*GetType();
		void SetWindowMoveListener(BaseWindow*listener);
		virtual bool IsVisiable();
		SplitLine*GetSplitLine(SplitLinePos pos);
		void ClearSplitLines();
		SplitLine*GetSplitLineToRemoveWhenCloseWindow();
		void ChangeWidthFromLeft(int targetDeltaWidth, int &deltaWidth);
		void ChangeWidthFromRight(int targetDeltaWidth, int &deltaWidth);
		void ChangeHeightFromBottom(int targetDeltaHeight, int &deltaHeight);
	public:
		static HINSTANCE mAppInstance; 
		SplitLine*mAttachedSplitLine;
		static void InitGlobalGUISettings(HINSTANCE instance);
		static ATOM RegisterWindowClass(UINT style, LPCTSTR pWndClassName, WNDPROC wndProc);
		static HWND CreateWindowWithStyle(WindowStyle style,int x,int y,int width,int height,LPCTSTR windowClass,HWND parent);
		static std::unordered_set<BaseWindow*> mScheduledWindows;
		static void SetRootWindow(BaseWindow*window);
		static void FixedUpdate();
		static bool mIsAppQuit;
		static bool mbNeedToUpdateSomeWindow;
		static int mThreadedWindowCount;
		static void PopThreadedWindow();
		static void PushThreadedWindow(); 
	};
}