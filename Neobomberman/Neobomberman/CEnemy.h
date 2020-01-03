#pragma once
#include<Windows.h>
#include"Define.h"

class CEnemy {
public:
	int nLife;
	float fXPos;
	float fYPos;

	DWORD dwLocation; 
	RECT rtCollision;

	DWORD dwAni;
	DWORD dwAniTime;
	DWORD dwMoveTime;
	DWORD dwState;

	ENEMY_KIND kind;
	MOVE_WAY way;

	void Move(DWORD dwCurTime);
	void SetAni(DWORD dwCurTime);
	void SetRECT();

	CEnemy(ENEMY_KIND kind, RECT rtCollision, DWORD dwLocation);
	~CEnemy();
};