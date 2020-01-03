#pragma once
#include<Windows.h>
#include"CGameFrame.h"

class CIntro : public CGameFrame {
private:
	static CIntro* inst;
	CIntro(HWND hWnd, HDC hBackbuffer);

	HDC hIntroDC;
	HDC hNumberDC;
	HDC hTextDC;

	GAME_STEP GameStep;

	DWORD dwCurTime;
	DWORD dwButtonTime;
	DWORD dwTextTime;
	DWORD dwCoin;

	bool bCoin;
public:
	static CIntro* getInstance(HWND hWnd, HDC hBackbuffer);

	void PressKey();
	void CheckCoin();

	void Init();
	GAME_STEP Update();
	void Render();
	~CIntro();
};