#pragma once
#include<Windows.h>
#include"Define.h"
#include"CGameFrame.h"
#include"CBattleManager.h"

class CBattleResult : public CGameFrame {
private:
	HDC hBackgroundDC;
	HDC hObjectDC;
	HDC hBomberDC;

	BATTLE_STEP battleStep;

	PLAYER winner;
	BOMBER bomber[2];

	int n1PWinCount;
	int n2PWinCount;

	DWORD dwCurTime;
	DWORD dwSecond;
	DWORD dwSecondTime;

	DWORD dwTextAni;
	DWORD dwTextAniTime;
	DWORD dwBomberAni;
	DWORD dwBomberAniTime;
	DWORD dwAudienceAni;
	DWORD dwAudienceAniTime;
public:
	bool bOn;

	void Init(PLAYER winner);
	void SetAni();
	BATTLE_STEP Update();
	void Render();

	CBattleResult(HWND hWnd, HDC hBackbuffer);
	~CBattleResult();
};
