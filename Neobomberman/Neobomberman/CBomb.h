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

	RECT rtHCollision; // ���� 
	RECT rtVCollision; // ���� �浹 ����

	void SetBomb(PLAYER player, DWORD dwLocation, DWORD dwBombRange);
	void SetAni(DWORD dwCurTime);
};