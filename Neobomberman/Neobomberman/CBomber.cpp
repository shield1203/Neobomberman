#include"CBomber.h"
#include"CSoundManager.h"

CBomber::CBomber() {
}

CBomber::~CBomber() {

}

void CBomber::InitBomber(PLAYER player, BOMBER bomber, float fSpeed) {
	bLife = true;

	this->player = player;
	this->bomber = bomber;

	direction = PLAYER_DOWN;
	state = P_STATE_MOVE;

	if (player == PLAYER_P1 || player == PLAYER_P1AI) { // 시작 좌표
		fXPos = 164;
		fYPos = 75;
		dwLocation = 0;
	}
	else {
		fXPos = 932;
		fYPos = 75;
		dwLocation = 12;
	}
	SetRECT();

	this->fSpeed = fSpeed;
	dwMaxBombCount = 1; 
	dwBombRange = 1;

	dwButtonTime = 0;
	dwAni = 0;
	dwAniTime = 0;
	dwDieCount = 0;
}

void CBomber::SetAni(DWORD dwCurTime) {
	switch (state) {
		case P_STATE_MOVE:
		{
			if (dwCurTime - dwAniTime > 120) {
				dwAniTime = dwCurTime;

				if (dwAni >= 192) {
					dwAni = 32;
				}
				else {
					dwAni += 32;
				}

				//CSoundManager::getInstance()->StartSubChEffect(SOUND_WALK); // 아니 왜 채널 나눴는데 소리가 묻히냐;
			}
			break;
		}
		case P_STATE_DIE: {
			if (dwAni == 256 && dwCurTime - dwAniTime > 50) {
				dwAniTime = dwCurTime;
				dwAni = 288;
			}
			else if (dwAni == 288 && dwCurTime - dwAniTime > 50) {
				dwAniTime = dwCurTime;
				dwAni = 256;
			}
			else {
				if (dwCurTime - dwAniTime > 85) {
					dwAniTime = dwCurTime;

					if (dwAni == 64) {
						if (dwDieCount != 1) {
							dwAni = 0;
							dwDieCount++;
						}
						else {
							dwAni += 32;
						}
					}
					else if (dwAni == 96) {
						if (dwDieCount != 2) {
							dwDieCount++;
						}
						else {
							dwAni += 32;
						}
						fYPos -= 20;
					}
					else if (dwAni == 160) {
						if (dwDieCount != 3) {
							dwDieCount++;
						}
						else {
							dwAni += 32;
						}
						fYPos += 20;
					}
					else {
						dwAni += 32;
					}
				}
			}
			break;
		}
		case P_STATE_DASH:
		{
			if (dwCurTime - dwAniTime > 60) {
				dwAniTime = dwCurTime;

				if (dwAni != 96) {
					dwAni = 96;
				}
			}
			break;
		}
	}
}

void CBomber::SetRECT() {
	rtCollision.left = fXPos + 28;
	rtCollision.right = rtCollision.left + 64;
	rtCollision.top = fYPos + 53;
	rtCollision.bottom = rtCollision.top + 64;

	pLocation.x = rtCollision.left + 32;
	pLocation.y = rtCollision.top + 32;
}