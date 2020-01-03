#include"CProcessManager.h"
#include<Vfw.h>

#pragma comment(lib, "vfw32.lib")

CProcessManager::CProcessManager(HWND hWnd) {
	GetWindowRect(hWnd, &WinRect);
	hProcWnd = hWnd;

	HBITMAP hBitmap;
	HDC hdc;
	gameStep = STEP_INTRO;

	hdc = GetDC(hWnd);
	hBackBuffer = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top);
	SelectObject(hBackBuffer, (HBITMAP)hBitmap);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdc);

	ModeSelect = nullptr;
}

CProcessManager::~CProcessManager() {
	DeleteDC(hBackBuffer);
}

void CProcessManager::Process() {
	Update();
	Render();
}

void CProcessManager::Update() {
	switch (gameStep)
	{
		case STEP_INTRO: {
			gameStep = CIntro::getInstance(hProcWnd, hBackBuffer)->Update();
			break;
		}
		case STEP_MODE_SELECT: {
			gameStep = ModeSelect->Update();
			break;
		}
		case STEP_BATTLE: {
			gameStep = CBattleManager::getInstance(hProcWnd, hBackBuffer)->Update();
			break;
		}
		case STEP_STORY: {
			gameStep = CStoryManager::getInstance(hProcWnd, hBackBuffer)->Update();
			break;
		}
	}

	if (gameStep != prevGameStep) {
		switch (prevGameStep) { 
			case STEP_MODE_SELECT: {
				delete ModeSelect;
				ModeSelect = nullptr;
				break;
			}
		}

		switch (gameStep) { // init
			case STEP_INTRO: {
				CIntro::getInstance(hProcWnd, hBackBuffer)->Init();
				break;
			}
			case STEP_MODE_SELECT: {
				if (ModeSelect == nullptr) {
					ModeSelect = new CModeSelect(hProcWnd, hBackBuffer);
				}
				break;
			}
			case STEP_BATTLE: {
				CBattleManager::getInstance(hProcWnd, hBackBuffer)->Init();
				break;
			}
			case STEP_STORY: {
				CStoryManager::getInstance(hProcWnd, hBackBuffer)->Init();
				break;
			}
		}
		prevGameStep = gameStep;
	}
}

void CProcessManager::Render() {
	HDC hdc = GetDC(hProcWnd);

	switch (gameStep)
	{
		case STEP_INTRO: {
			CIntro::getInstance(hProcWnd, hBackBuffer)->Render();
			break;
		}
		case STEP_MODE_SELECT: {
			ModeSelect->Render();
			break;
		}
		case STEP_BATTLE: {
			CBattleManager::getInstance(hProcWnd, hBackBuffer)->Render();
			break;
		}
		case STEP_STORY: {
			CStoryManager::getInstance(hProcWnd, hBackBuffer)->Render();
			break;
		}
	}

	BitBlt(hdc, 0, 0, WinRect.right - WinRect.left, WinRect.bottom - WinRect.top, hBackBuffer, 0, 0, SRCCOPY);
	ReleaseDC(hProcWnd, hdc);
}