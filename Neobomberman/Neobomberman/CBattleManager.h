#pragma once
#include<Windows.h>
#include<string>
#include"Define.h"
class CMapSelect;
class CBomberSelect;
class CBattle;
class CBattleResult;

class CBattleManager {
private:
	static CBattleManager* inst;

	CBattleManager(HWND hWnd, HDC hBackbuffer);
public:
	static CBattleManager* getInstance(HWND hWnd, HDC hBackbuffer);

	HWND hWnd;
	HDC hBackbuffer;

	GAME_STEP gameStep;
	BATTLE_STEP battleStep;

	CMapSelect *MapSelect;
	CBomberSelect *BomberSelect;
	CBattle *Battle;
	CBattleResult *BattleResult;

	void Init();
	GAME_STEP Update();
	void Render();

	~CBattleManager();
};