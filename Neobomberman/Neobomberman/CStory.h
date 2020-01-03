#pragma once
#include<Windows.h>
#include<cstdlib>
#include<ctime>
#include"CGameFrame.h"
#include"CBomber.h"
#include"CBomb.h"
#include"CItem.h"
#include"CEnemy.h"
#include"Define.h"
#include"tinyxml.h"
#include<string>
#include<vector>

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

class CStory : public CGameFrame{
private:
	HDC h1PDC;
	HDC hClearDC;
	HDC hEnemyDC;
	HDC hInterfaceDC;
	HDC hNumDC;
	HDC hItemDC;
	HDC hBombDC;
	HDC hBackgroundDC;
	HDC hTyleDC[22];
	HDC hObjectDC[7];
	HDC hBlockDC[3];

	STORY_STEP storyStep;

	MapBlock mapBlock[143];

	DWORD dwCurTime;
	DWORD dwSecond;
	DWORD dwSecondTime;
	DWORD dwDieTime;
	DWORD dwPushStartTime;
	int nPushStart;

	CBomber bomber;
	int nScore;
	vector<CBomb*> bomb;
	vector<CItem*> item;
	CEnemy *Enemy[3];

	bool bClear;
	bool bFail;
	DWORD dwResultTime;
	DWORD dwClearAni;
	int nResultCount;
public:
	void MapDCLoad();
	void LoadMapData(string strMapFileName);
	void BomberControl();
	void LocationCheck();

	void CheckResult();
	void CheckBombRange(int nBombIndex); // ÆøÆÈ¹üÀ§ Ã¼Å©
	void CheckCollision();
	void SetBlockAni();
	void SetClearAni();

	STORY_STEP Update();
	void Render();

	CStory(HWND hWnd, HDC hBackbuffer);
	~CStory();
};
