#pragma once

#include<Windows.h>
#include"CIntro.h"
#include"CModeSelect.h"
#include"CBattleManager.h"
#include"CStoryManager.h"
#include "Define.h"

class CProcessManager { // ¿Ã∞≈ ΩÃ±€≈Ê¿∏∑Œ
private:
	HWND hProcWnd;
	HDC hBackBuffer;
	RECT WinRect;

	CModeSelect *ModeSelect;

	GAME_STEP gameStep, prevGameStep;
public:
	void Process();
	void Update();
	void Render();

	CProcessManager(HWND hWnd);
	~CProcessManager();
};