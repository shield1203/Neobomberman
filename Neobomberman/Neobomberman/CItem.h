#pragma once
#include<Windows.h>
#include"Define.h"

class CItem {
public:
	bool bOn;

	ITEM kind;

	RECT rtCollision;

	DWORD dwLocation;
	DWORD dwAni;
	DWORD dwAniTime;

	CItem();
	void Init(ITEM kind, DWORD dwLocation, RECT rtCollision);
	void SetAni(DWORD dwCurtime);
};