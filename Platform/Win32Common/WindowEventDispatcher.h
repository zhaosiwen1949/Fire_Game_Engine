#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Utils/LinkedList.h"
namespace Editor{
	class WindowEventDispatcher : public Alice::LinkedList{
	protected:
		bool mbMouseLeaved;
		bool mbIsIMEInputMode;
		bool mbIsIMEInputModeDelay;
		HWND mhWnd;
	protected:
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMiddleButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMiddleButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnRButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnRButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonDoubleClick(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);

		virtual void OnCloseWindow(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnFocus(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMoving(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnPaint();
		virtual void OnTimer(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnDropFiles(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCLBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCLBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCLBUTTONDBLCLK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCMOUSEMOVE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCRBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCRBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		WindowEventDispatcher();
		static LRESULT CALLBACK WindowEventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}