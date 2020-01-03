#pragma once
#include<Windows.h>
#include"CGameFrame.h"
#include"Define.h"

struct Selector {
	SELECTOR_STATE state = SELECTOR_IDLE;
	MOVE_WAY way = WAY_STOP;
	bool bSelect = false;

	float fXPos = 40;
	float fYPos = 380;

	DWORD dwSelectorAni = 0;
	DWORD dwSelectorAniTime = 0;
	DWORD dwSelectAni = 0;
	DWORD dwSelectAniTime = 0;
};

class CModeSelect : public CGameFrame {
private:
	HDC hBackgroundStoryDC;
	HDC hBackgroundBattleDC;
	HDC hSelectorDC;

	GAME_STEP gameStep;

	Selector selector;
	SELECT_MODE mode; // � ��������� ���� ���

	DWORD dwCurTime; // ��� ���ŵǴ� �ð�
	DWORD dwButtonTime;
	DWORD dwMoveTime;
public:
	CModeSelect(HWND hWnd, HDC hBackbuffer);

	void CheckKey();
	void SetAni();
	void Move();

	GAME_STEP Update();
	void Render();

	~CModeSelect();
};