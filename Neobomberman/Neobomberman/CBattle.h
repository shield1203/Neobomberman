#pragma once
#include<Windows.h>
#include"CBattleManager.h"
#include"CGameFrame.h"
#include"CBomber.h"
#include"CBomb.h"
#include"CItem.h"
#include"Define.h"
#include"tinyxml.h"
#include<string>
#include<vector>
#include<ctime>

using namespace std;

struct MapBlock {
	bool bOn = false;
	bool bMelting = false;

	DWORD dwAni = 0;
	DWORD dwAniTime = 0;

	RECT rtBlock;
	TYLE Tyle = NONE_TYLE;
	OBJECT Object = NONE_OBJECT;
	BLOCK Block = NONE_BLOCK;
};

class CBattle : public CGameFrame {
private:
	HDC hInterfaceDC;
	HDC hNumDC;
	HDC hItemDC;
	HDC hBombDC;
	HDC hBomberDC[12];
	HDC hBackgroundDC[4];
	HDC hTyleDC[22];
	HDC hObjectDC[7];
	HDC hBlockDC[3];

	BATTLE_STEP battleStep;

	MapBlock mapBlock[143]; // ºí·Ï´ç ¸Ê Á¤º¸

	DWORD dwBackground;
	DWORD dwCurTime;
	DWORD dwSecond; 
	DWORD dwSecondTime; 
	DWORD dwDieTime;

public:
	bool bOn;

	CBomber bomber[2];
	vector<CBomb*> bomb;
	vector<CItem*> item;

	void Init();
	void MapDCLoad();
	void LoadMapData(string strMapFileName);
	
	void BomberControl();
	void LocationCheck();

	void CheckBombRange(int nBombIndex); // ÆøÆÈ¹üÀ§ Ã¼Å©
	void CheckCollision();
	void SetBlockAni();

	BATTLE_STEP Update();
	void Render();

	CBattle(HWND hWnd, HDC hBackbuffer);
	~CBattle();
};