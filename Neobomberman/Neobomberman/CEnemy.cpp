#include"CEnemy.h"

CEnemy::CEnemy(ENEMY_KIND kind, RECT rtCollision, DWORD dwLocation) {
	this->kind = kind;
	this->rtCollision.left = rtCollision.left;
	this->rtCollision.right = rtCollision.right;
	this->rtCollision.top = rtCollision.top;
	this->rtCollision.bottom = rtCollision.bottom;
	this->dwLocation = dwLocation;

	fXPos = rtCollision.left - 28;
	fYPos = rtCollision.top - 53;

	nLife = 1;

	way = (MOVE_WAY)(rand() % 4 + 1);

	dwAni = 0;
	dwAniTime = 0;
	dwMoveTime = 0;
	dwState = 0; // ¸¶Åº°í : µÚ 0, ¿À¸¥ 32, ¾Õ 64, ¿Þ 96, ¹»±î 128 . 352
}

CEnemy::~CEnemy() {

}

void CEnemy::Move(DWORD dwCurTime) {
	switch (way) {
		case WAY_UP: {
			if (kind == Matango) {
				dwState = 32;
			}
			if (dwCurTime - dwMoveTime > 50) {
				dwMoveTime - dwCurTime;

				fYPos -= 1;
				SetRECT();
			}
			break;
		}
		case WAY_DOWN: {
			if (kind == Matango) {
				dwState = 64;
			}
			if (dwCurTime - dwMoveTime > 50) {
				dwMoveTime - dwCurTime;

				fYPos += 1;
				SetRECT();
			}
			break;
		}
		case WAY_LEFT: {
			if (kind == Matango) {
				dwState = 96;
			}
			if (dwCurTime - dwMoveTime > 50) {
				dwMoveTime - dwCurTime;

				fXPos -= 1;
				SetRECT();
			}
			break;
		}
		case WAY_RIGHT: {
			if (kind == Matango) {
				dwState = 32;
			}
			if (dwCurTime - dwMoveTime > 50) {
				dwMoveTime - dwCurTime;

				fXPos += 1;
				SetRECT();
			}
			break;
		}
	}
}

void CEnemy::SetAni(DWORD dwCurTime) {
	switch (kind)
	{
		case Matango: {
			if (dwCurTime - dwAniTime > 110) {
				dwAniTime = dwCurTime;

				if (dwState != 128) {
					if (dwAni < 192) {
						dwAni += 32;
					}
					else {
						dwAni = 0;
					}
				}
				else {
					if (dwAni < 352) {
						dwAni += 32;
					}
					else {
						dwAni = 0;
					}
				}
			}
			break;
		}
		case SeekerBalloon: {
			if (dwCurTime - dwAniTime > 250) {
				dwAniTime = dwCurTime;

				if (dwAni < 224) {
					dwAni += 32;
				}
				else {
					dwAni = 0;
				}
			}
			break;
		}
		case GingerBreadman: {
			if (dwCurTime - dwAniTime > 200) {
				dwAniTime = dwCurTime;

				if (dwAni < 224) {
					dwAni += 32;
				}
				else {
					dwAni = 0;
				}
			}
			break;
		}
	}
}

void CEnemy::SetRECT() {
	rtCollision.left = fXPos + 28;
	rtCollision.right = rtCollision.left + 64;
	rtCollision.top = fYPos + 53;
	rtCollision.bottom = rtCollision.top + 64;
}