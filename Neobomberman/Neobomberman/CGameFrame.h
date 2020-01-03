#pragma once
#include<Windows.h>
#include"Define.h"

class CGameFrame {
public:
	HWND hWnd;
	HDC hBackbuffer;
	HDC hFadeDC;

	bool bFadeIn;
	bool bFadeOut;

	int nAlpha;
	DWORD dwAlphaTime;
	BLENDFUNCTION BlendFunction;

	HDC CreateBmpDC(const char* filename);
	
	void FadeOut();
	void FadeIn();
	//void EndGame();

	CGameFrame(HWND hWnd, HDC hBackbuffer);
	~CGameFrame();
};