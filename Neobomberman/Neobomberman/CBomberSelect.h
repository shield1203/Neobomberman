#pragma once
#include<Windows.h>
#include"CBattleManager.h"
#include"CGameFrame.h"
#include"Define.h"

struct BomberSelector {
	PLAYER player;

	bool bPSelect = false;
	bool bReady = false;

	DWORD dwSelectorAni = 0;
	DWORD dwSelectorAniTime = 0;
	DWORD dwBombermanAni = 0;
	DWORD dwBombermanAniTime = 0;
	DWORD dwButtonTime = 0;

	int nPRow = 0;
	int nPColumn = 0;
	BOMBER bomber;

	int key = VK_RIGHT; // 입력된 키 값이 무엇인지 저장
};

class CBomberSelect : public CGameFrame {
private:
	HDC hBackgroundDC;
	HDC hBomberNameDC;
	HDC hSelectorDC;
	HDC hBombermansDC;

	DWORD dwCurTime;
public:
	bool bOn;

	BomberSelector Player[2];

	void CheckKey();
	void CheckSelect(); // 같은 봄버가 선택되는걸 방지
	void SetAni();

	void Init();
	void Update();
	void Render();

	CBomberSelect(HWND hWnd, HDC hBackbuffer);
	~CBomberSelect();
};