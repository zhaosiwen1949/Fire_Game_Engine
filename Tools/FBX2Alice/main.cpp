#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#include "glew.h"
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"LibProtoBufferLite.lib")
#include "scene.h"
#include "XVulkan.h"
#include "fbxskeletoncpu.h"
unsigned char * LoadFileContent(const char *path, int &filesize) {
	unsigned char*fileContent = nullptr;
	filesize = 0;
	FILE*pFile = fopen(path, "rb");
	if (pFile) {
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0) {
			rewind(pFile);
			fileContent = new unsigned char[nLen + 1];
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return fileContent;
}
float GetFrameTime() {
	static unsigned long lastTime = 0, timeSinceComputerStart = 0;
	timeSinceComputerStart = timeGetTime();
	unsigned long frameTime = lastTime == 0 ? 0 : timeSinceComputerStart - lastTime;
	lastTime = timeSinceComputerStart;
	return float(frameTime) / 1000.0f;
}
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		OnViewportChanged(rect.right - rect.left, rect.bottom - rect.top);
	}
	break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszClassName = L"BattleFireWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom) {
		MessageBox(NULL, L"Register Fail", L"Error", MB_OK);
		return 0;
	}
	RECT rect = { 0,0,1280,720 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd = CreateWindowEx(NULL, L"BattleFireWindow", L"LearnWindow", WS_OVERLAPPEDWINDOW,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr);
	xInitVulkan(hwnd, 1280, 720);
	Init();
	Alice::FBXSkeletonCPU fbx;
	fbx.Init("tauren.fbx");
	fbx.InitForGeneral();
	fbx.BakeTPose("tauren.avatar");
	fbx.BakeAnimationFromMathDescriptionToModel("Run", "Run.animation");
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	MSG msg;
	float last_time = timeGetTime() / 1000.0f;
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		float current_time = timeGetTime() / 1000.0f;
		float deltaTime = current_time - last_time;
		last_time = current_time;
		Draw(deltaTime);
	}
	OnQuit();
	return 0;
}