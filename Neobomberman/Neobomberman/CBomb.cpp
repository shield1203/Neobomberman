#include"CBomb.h"
#include"CSoundManager.h"

void CBomb::SetBomb(PLAYER player, DWORD dwLocation, DWORD dwBombRange) {
	bOn = true;
	bExplosion = false;
	dwAni = 0;
	dwAniTime = 0;

	this->player = player;
	this->dwLocation = dwLocation;
	dwRange = dwBombRange;
}

void CBomb::SetAni(DWORD dwCurTime) {
	if (bExplosion) { // ÆøÆÈ ÈÄ
		if (player != NONE_PLAYER) {
			CSoundManager::getInstance()->StartChEffect(SOUND_BOOM);
		}

		player = NONE_PLAYER;
		if (dwCurTime - dwAniTime > 40) {
			dwAniTime = dwCurTime;

			dwAni += 16;		
			
			if (dwAni == 192) {
				bOn = false;
				bExplosion = false;
			}
		}
	}
	else { // ÆøÆÈ Àü, ÆøÅº»óÅÂ
		if (dwCurTime - dwAniTime > 280) {
			dwAniTime = dwCurTime;

			dwAni += 16;

			if (dwAni == 144) {
				dwAni = 0;
				bExplosion = true;
			}
		}

	}
}