#pragma once
#include<Windows.h>
#include"Define.h"

class CBomber {
public:
	PLAYER player;
	
	bool bLife;

	BOMBER bomber;
	PLAYER_DIRECTION direction;
	PLAYER_STATE state;

	float fXPos;
	float fYPos;
	float fSpeed;
	RECT rtCollision;
	POINT pLocation;
	DWORD dwLocation;

	// ������ ����
	//bool bBomb;
	DWORD dwMaxBombCount; // �ִ� ��ź ��
	DWORD dwBombRange; // ��ź ����

	DWORD dwButtonTime;
	DWORD dwAni;
	DWORD dwAniTime;
	DWORD dwDieCount;

	void InitBomber(PLAYER player, BOMBER bomber, float fSpeed);
	void SetAni(DWORD dwCurTime);
	void SetRECT();

	CBomber();
	~CBomber();
};