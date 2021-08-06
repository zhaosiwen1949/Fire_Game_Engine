#include "BaseWindow.h"
#include "Runtime/Debugger/Log.h"
#include "SplitLine.h"
HINSTANCE Editor::BaseWindow::mAppInstance;
#pragma comment(lib,"gdiplus.lib")
namespace Editor
{
	int BaseWindow::mThreadedWindowCount = 0;
	bool BaseWindow::mIsAppQuit = false;
	bool BaseWindow::mbNeedToUpdateSomeWindow = false;
	std::unordered_set<BaseWindow*> BaseWindow::mScheduledWindows;
	void BaseWindow::PushThreadedWindow()
	{
		mThreadedWindowCount++;
	}

	void BaseWindow::PopThreadedWindow()
	{
		mThreadedWindowCount--;
	}

	void BaseWindow::InitGlobalGUISettings(HINSTANCE instance){
		static GdiplusStartupInput sGdiplusStartupInput;
		static ULONG_PTR sGdiplusToken;
		Editor::BaseWindow::mAppInstance = instance;
		GdiplusStartup(&sGdiplusToken, &sGdiplusStartupInput, NULL);
		CoInitialize(NULL);
	}

	BaseWindow::BaseWindow() : mbVisiable(false), mbCloseForMaximize(false),
		mLeftSplitLine(nullptr), mRightSplitLine(nullptr), mUpperSplitLine(nullptr), mBottomSplitLine(nullptr),
		mbHasMiniSize(false), mParent(nullptr), mAttachedSplitLine(nullptr){
		SetRect(0, 0, 800, 600);
		SetMinRect(0, 0, 200, 200);
		mbDirty = true;
		mBKGColor = Color(41,41,41);
		memset(mName,0,64);
	}

	BaseWindow::~BaseWindow()
	{
		DestroyWindow(mhWnd);
	}


	void BaseWindow::SetMinRect(int x, int y, int width, int height)
	{
		mbHasMiniSize = true;
		mMinRect.X = x;
		mMinRect.Y = y;
		mMinRect.Width = width;
		mMinRect.Height = height;
	}

	Rect & BaseWindow::GetMinRect()
	{
		return mMinRect;
	}

	int BaseWindow::GetMinWidth()
	{
		return mMinRect.Width;
	}

	int BaseWindow::GetMinHeight()
	{
		return mMinRect.Height;
	}

	static BaseWindow*sRootWindow=nullptr;

	void BaseWindow::ScheduleUpdate()
	{
		if (sRootWindow!=this)
		{
			sRootWindow->PushBack(this);
			Debug("schedule window %s", mName);
		}
	}

	void BaseWindow::CancelUpdate()
	{
	}

	void BaseWindow::SetRootWindow(BaseWindow*window)
	{
		sRootWindow = window;
	}

	void BaseWindow::FixedUpdate()
	{
		if (sRootWindow != nullptr) {
			sRootWindow->PaintWindow();
		}
	}


	void BaseWindow::DrawContent(Gdiplus::Graphics&painter)
	{

	}

	void BaseWindow::OnEndPaint()
	{

	}

	void BaseWindow::SetHWND_DC(HWND hwnd, HDC hdc)
	{
		mhWnd = hwnd;
		mHDC = hdc;
	}

	void BaseWindow::OnPaint()
	{
		Gdiplus::Graphics painter(mHDC);
		painter.Clear(mBKGColor);
		DrawContent(painter);
		OnEndPaint();
	}

	void BaseWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved)
	{
		mRect.Width = LOWORD(lParam);
		mRect.Height = HIWORD(lParam);
	}

	void BaseWindow::OnMove(WPARAM wParam, LPARAM lParam, void*reserved)
	{
		//Debug("%s who moves",mName);
	}

	void BaseWindow::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (mbHasMiniSize)
		{
			MINMAXINFO *ptr = (MINMAXINFO*)lParam;
			ptr->ptMinTrackSize.x = mMinRect.Width;
			ptr->ptMinTrackSize.y = mMinRect.Height;
		}
	}

	void BaseWindow::OnWindowMove(BaseWindow*movedWindow)
	{

	}

	void BaseWindow::OnEditControlLoseFocus(BaseWindow*editControl)
	{
	}

	void BaseWindow::SetWindowMoveListener(BaseWindow*listener)
	{
		mWindowMoveEventListeners.insert(listener);
	}


	bool BaseWindow::IsVisiable()
	{
		if (mParent!=nullptr)
		{
			return mbVisiable&&mParent->IsVisiable();
		}
		return mbVisiable;
	}

	SplitLine*BaseWindow::SetSplitLine(SplitLinePos pos, SplitLine*splitLine)
	{
		SplitLine*ret = nullptr;
		switch (pos)
		{
		case SplitLinePosLeft:
			ret = mLeftSplitLine;
			mLeftSplitLine = splitLine;
			break;
		case SplitLinePosRight:
			ret = mRightSplitLine;
			mRightSplitLine = splitLine;
			break;
		case SplitLinePosUpper:
			ret = mUpperSplitLine;
			mUpperSplitLine = splitLine;
			break;
		case SplitLinePosBottom:
			ret = mBottomSplitLine;
			mBottomSplitLine = splitLine;
			break;
		}
		return ret;
	}

	void BaseWindow::ClearSplitLines()
	{
		if (mLeftSplitLine!=nullptr)
		{
			mLeftSplitLine->RemoveWindow(SplitLinePosLeft, this);
		}
		if (mRightSplitLine!=nullptr)
		{
			mRightSplitLine->RemoveWindow(SplitLinePosRight, this);
		}
		if (mUpperSplitLine!=nullptr)
		{
			mUpperSplitLine->RemoveWindow(SplitLinePosUpper, this);
		}
		if (mBottomSplitLine!=nullptr)
		{
			mBottomSplitLine->RemoveWindow(SplitLinePosBottom, this);
		}
		mLeftSplitLine = nullptr;
		mRightSplitLine = nullptr;
		mUpperSplitLine = nullptr;
		mBottomSplitLine = nullptr;
	}

	SplitLine*BaseWindow::GetSplitLine(SplitLinePos pos)
	{
		switch (pos)
		{
		case SplitLinePosLeft:
			return mLeftSplitLine;
		case SplitLinePosRight:
			return mRightSplitLine;
		case SplitLinePosUpper:
			return mUpperSplitLine;
		case SplitLinePosBottom:
			return mBottomSplitLine;
		}
		return nullptr;
	}


	void BaseWindow::ChangeWidthFromLeft(int targetDeltaWidth, int &deltaWidth)
	{
		//Debug("reduce %d %d -> %d",targetDeltaWidth,mRect.Width,mMinRect.Width);
		if (targetDeltaWidth<0)
		{
			if ((mRect.Width + targetDeltaWidth) >= mMinRect.Width)
			{
				deltaWidth += targetDeltaWidth;
				MoveWindow(mRect.X, mRect.Y, mRect.Width + targetDeltaWidth, mRect.Height);
				return;
			}
			else if (mRect.Width >= mMinRect.Width)
			{
				int nDeltaWidthByThis = 0;
				int nDeltaWidthByLeftWindows = 0;
				int nX = mRect.X, nWidth = mMinRect.Width;

				nDeltaWidthByThis = mMinRect.Width - mRect.Width;
				deltaWidth += nDeltaWidthByThis;

				targetDeltaWidth -= nDeltaWidthByThis;

				nDeltaWidthByLeftWindows = 0;
				int tempWidth = 0;
				if (mLeftSplitLine != nullptr)
				{
					auto iter = mRightSplitLine->RightWindows().begin();
					auto iterEnd = mRightSplitLine->RightWindows().end();
					(*iter++)->ChangeWidthFromRight(targetDeltaWidth, nDeltaWidthByLeftWindows);
					while (iterEnd != iter)
					{
						//reduce my left side windows
						(*iter++)->ChangeWidthFromRight(targetDeltaWidth, tempWidth);
					}
				}
				if (nDeltaWidthByLeftWindows != 0)
				{
					nX += nDeltaWidthByLeftWindows;
					deltaWidth += nDeltaWidthByLeftWindows;
				}
				MoveWindow(nX, mRect.Y, nWidth, mRect.Height);
			}
		}
		else
		{
			deltaWidth += targetDeltaWidth;
			MoveWindow(mRect.X , mRect.Y, mRect.Width + targetDeltaWidth, mRect.Height);
			targetDeltaWidth = 0;
		}
	}

	void BaseWindow::ChangeWidthFromRight(int targetDeltaWidth, int &deltaWidth)
	{
		//Debug("reduce %d %d -> %d",targetDeltaWidth,mRect.Width,mMinRect.Width);
		if (targetDeltaWidth < 0)
		{
			if ((mRect.Width + targetDeltaWidth) >= mMinRect.Width)
			{
				deltaWidth += targetDeltaWidth;
				MoveWindow(mRect.X, mRect.Y, mRect.Width + targetDeltaWidth, mRect.Height);
				return;
			}
			else if(mRect.Width>=mMinRect.Width)
			{
				int nDeltaWidthByThis = 0;
				int nDeltaWidthByLeftWindows = 0;
				int nX = mRect.X, nWidth = mMinRect.Width;

				nDeltaWidthByThis = mMinRect.Width - mRect.Width;
				deltaWidth += nDeltaWidthByThis;

				targetDeltaWidth -= nDeltaWidthByThis;

				nDeltaWidthByLeftWindows = 0;
				int tempWidth = 0;
				if (mLeftSplitLine != nullptr)
				{
					auto iter = mLeftSplitLine->LeftWindows().begin();
					auto iterEnd = mLeftSplitLine->LeftWindows().end();
					(*iter++)->ChangeWidthFromRight(targetDeltaWidth, nDeltaWidthByLeftWindows);
					while (iterEnd != iter)
					{
						//reduce my left side windows
						(*iter++)->ChangeWidthFromRight(targetDeltaWidth, tempWidth);
					}
				}
				if (nDeltaWidthByLeftWindows != 0)
				{
					nX += nDeltaWidthByLeftWindows;
					deltaWidth += nDeltaWidthByLeftWindows;
				}
				MoveWindow(nX, mRect.Y, nWidth, mRect.Height);
			}
		}
		else
		{
			deltaWidth += targetDeltaWidth;
			MoveWindow(mRect.X, mRect.Y, mRect.Width + targetDeltaWidth, mRect.Height);
		}
	}

	void BaseWindow::ChangeHeightFromBottom(int targetDeltaHeight, int &deltaHeight)
	{
		if (targetDeltaHeight < 0)
		{
			if ((mRect.Height + targetDeltaHeight) >= mMinRect.Height)
			{
				deltaHeight += targetDeltaHeight;
				MoveWindow(mRect.X, mRect.Y, mRect.Width , mRect.Height + targetDeltaHeight);
				return;
			}
			else if (mRect.Height >= mMinRect.Height)
			{
				int nDeltaHeightByThis = 0;
				int nDeltaHeightByUpperWindows = 0;
				int nY = mRect.Y, nHeight = mMinRect.Height;

				nDeltaHeightByThis = mMinRect.Height - mRect.Height;
				deltaHeight += nDeltaHeightByThis;

				targetDeltaHeight -= nDeltaHeightByThis;

				nDeltaHeightByUpperWindows = 0;
				int tempWidth = 0;
				if (mUpperSplitLine != nullptr)
				{
					auto iter = mUpperSplitLine->UpperWindows().begin();
					auto iterEnd = mUpperSplitLine->UpperWindows().end();
					(*iter++)->ChangeHeightFromBottom(targetDeltaHeight, nDeltaHeightByUpperWindows);
					while (iterEnd != iter)
					{
						//reduce my upper side windows
						(*iter++)->ChangeHeightFromBottom(targetDeltaHeight, tempWidth);
					}
				}
				if (nDeltaHeightByUpperWindows != 0)
				{
					nY += nDeltaHeightByUpperWindows;
					deltaHeight += nDeltaHeightByUpperWindows;
				}
				MoveWindow(mRect.X, nY, mRect.Width, nHeight);
			}
		}
		else
		{
			deltaHeight += targetDeltaHeight;
			MoveWindow(mRect.X, mRect.Y, mRect.Width , mRect.Height + targetDeltaHeight);
		}
	}

	SplitLine* BaseWindow::GetSplitLineToRemoveWhenCloseWindow()
	{
		//int left = mLeftSplitLine->mRightWindows.size();
		//int right = mRightSplitLine->mLeftWindows.size();
		//int upper = mUpperSplitLine->mBottomWindows.size();
		//int bottom = mBottomSplitLine->mUpperWindows.size();
		return nullptr;
	}

	void BaseWindow::SetParent(BaseWindow*parent)
	{
		if (parent != nullptr)
		{
			::SetParent(mhWnd, parent->GetHwnd());
		}
		mParent = parent;
	}

	void BaseWindow::SetBkgColor(Color &color)
	{
		mBKGColor = color;
	}

	void BaseWindow::Update()
	{
		mbDirty = true;//indicate that this window is dirty
		mbNeedToUpdateSomeWindow = true;//indicate that some window in the linked windows is dirty
	}

	void BaseWindow::PaintWindow(){
		if (mbDirty){
			OnPaint();
			mbDirty = false;
		}
		if (mNext != nullptr){
			Next<BaseWindow>()->PaintWindow();
		}
	}

	int BaseWindow::GetX()
	{
		return mRect.X;
	}

	int BaseWindow::GetY()
	{
		return mRect.Y;
	}

	int BaseWindow::GetWidth()
	{
		return mRect.Width;
	}

	int BaseWindow::GetHeight()
	{
		return mRect.Height;
	}

	void BaseWindow::SetRect(int x, int y, int width, int height)
	{
		mRect.X = x;
		mRect.Y = y;
		mRect.Width = width;
		mRect.Height = height;
	}

	void BaseWindow::SetRect(Rect &rect)
	{
		mRect = rect;
	}

	Rect BaseWindow::GetRect()
	{
		return mRect;
	}

	void BaseWindow::MoveWindow(int x, int y, int width, int height)
	{
		SetRect(x, y, width, height);
		::MoveWindow(mhWnd, x, y, width, height,true);
		if (mAttachedSplitLine != nullptr)
		{
			if (mAttachedSplitLine == mLeftSplitLine)
			{
				mAttachedSplitLine->SetRect(mRect.X - 6, mRect.Y, 6, mRect.Height);
			}
			else if (mAttachedSplitLine == mUpperSplitLine)
			{
				mAttachedSplitLine->SetRect(mRect.X, mRect.Y - 6, mRect.Width, 6);
			}
		}
	}

	void BaseWindow::SetSize(int width, int height,HWND param)
	{
		mRect.Width = width;
		mRect.Height = height;
		SetWindowPos(mhWnd, param, 0, 0, mRect.Width, mRect.Height, SWP_SHOWWINDOW);
	}

	BaseWindow*BaseWindow::GetParent()
	{
		return mParent;
	}

	HWND BaseWindow::GetHwnd()
	{
		return mhWnd;
	}

	const char*BaseWindow::GetWindowName()
	{
		return mName;
	}

	void BaseWindow::SetWindowName(const char*name)
	{
		memset(mName, 0, 64);
		strcpy(mName, name);
	}

	void BaseWindow::SetType(const char* typeName)
	{
		memset(mType, 0, 64);
		strcpy(mType, typeName);
	}

	const char*BaseWindow::GetType()
	{
		return mType;
	}

	void BaseWindow::Show(bool bShow)
	{
		mbVisiable = bShow;
		ShowWindow(mhWnd, bShow?SW_SHOW:SW_HIDE);
		UpdateWindow(mhWnd);
		OnShow(mbVisiable);
	}

	void BaseWindow::ShowOnTop()
	{
		mbVisiable = true;
		SetWindowPos(mhWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		OnShow(mbVisiable);
		Update();
	}

	void BaseWindow::OnShow(bool bShow)
	{
	}

	HDC BaseWindow::GetDC()
	{
		return mHDC;
	}

	ATOM BaseWindow::RegisterWindowClass(UINT style, LPCTSTR pWndClassName, WNDPROC wndProc)
	{
		WNDCLASSEXW wcex;
		memset(&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = style;
		wcex.lpfnWndProc = wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = mAppInstance;
		wcex.hIcon = NULL;
		wcex.hIconSm = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = pWndClassName;
		ATOM atom = RegisterClassExW(&wcex);
		if (!atom)
		{
			return 0;
		}
		return atom;
	}

	HWND BaseWindow::CreateWindowWithStyle(WindowStyle style, int x, int y, int width, int height, LPCTSTR windowClass, HWND parent)
	{
		if (style==WindowStyleNormalWindow)
		{
			DWORD windowStyle = WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN;
			HWND hwnd = CreateWindowEx(NULL,windowClass,L"Window",windowStyle,x,y,width,height,parent,NULL,mAppInstance,NULL);
			return hwnd;
		}
		else if (style==WindowStyleContainerWindow)
		{
			DWORD windowStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME;
			HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE | WS_EX_TOPMOST, windowClass, L"Window", windowStyle, x, y, width, height, parent, NULL, mAppInstance, NULL);
			return hwnd;
		}
		else if (style==WindowStyleAuxWindow)
		{
			DWORD windowStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW , windowClass, L"Window", windowStyle, x, y, width, height, parent, NULL, mAppInstance, NULL);
			return hwnd;
		}
		else if (style==WindowStyleDlgWindow)
		{
			DWORD windowStyle = WS_POPUP | WS_DLGFRAME| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			HWND hwnd = CreateWindowEx(WS_EX_TOPMOST , windowClass, L"Window", windowStyle, x, y, width, height, parent, NULL, mAppInstance, NULL);
			return hwnd;
		}
		return nullptr;
	}
}