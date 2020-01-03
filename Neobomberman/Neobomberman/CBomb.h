#pragma once
#include<Windows.h>
#include"Define.h"

class CBomb {
public:
	bool bOn;
	bool bExplosion;

	PLAYER player;

	DWORD dwLocation;
	DWORD dwRange;
	DWORD dwUp;
	DWORD dwDown;
	DWORD dwLeft;
	DWORD dwRight;

	DWORD dwAni;
	DWORD dwAniTime;

	RECT rtHCollision; // 가로 
	RECT rtVCollision; // 세로 충돌 범위

	void SetBomb(PLAYER player, DWORD dwLocation, DWORD dwBombRange);
	void SetAni(DWORD dwCurTime);
};