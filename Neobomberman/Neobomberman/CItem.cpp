#include"CItem.h"

CItem::CItem() {
	bOn = false;
	dwAni = 0;
	dwAniTime = 0;
}

void CItem::Init(ITEM kind, DWORD dwLocation, RECT rtCollision) {
	bOn = true;
	this->kind = kind;
	this->dwLocation = dwLocation;
	this->rtCollision = rtCollision;

	this->rtCollision.left += 20;
	this->rtCollision.right -= 20;
	this->rtCollision.top += 20;
	this->rtCollision.bottom -= 20;
}

void CItem::SetAni(DWORD dwCurtime) {
	if (dwCurtime - dwAniTime > 100) {
		dwAniTime = dwCurtime;

		dwAni += 64;

		if (dwAni == 512) {
			dwAni = 0;
		}
	}
}