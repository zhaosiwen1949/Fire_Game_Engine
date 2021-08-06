#pragma once
#include "Platform/Win32Common/DoubleBufferedWindow.h"
namespace Editor {
	class ContainerWindow :public DoubleBufferedWindow {
	protected:
		Point mDeltaWhenDrag;
		int mMarginLeft, mMarginRight, mMarginTop, mMarginBottom;
		bool mbDraging;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		ContainerWindow();
		void Init();
		static void InitWindowClasses();
	};
}