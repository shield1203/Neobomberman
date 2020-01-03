#include"CBattleResult.h"
#include"CBomberSelect.h"
#include"CSoundManager.h"

CBattleResult::CBattleResult(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	bOn = false;

	hBackgroundDC = CreateBmpDC("Resource\\BattleResult\\Background.bmp");
	hObjectDC = CreateBmpDC("Resource\\BattleResult\\Object.bmp");
	hBomberDC = CreateBmpDC("Resource\\BattleResult\\Bomber.bmp");

	battleStep = BATTLE_RESULT;

	n1PWinCount = 0;
	n2PWinCount = 0;

	dwSecond = 0;
	dwSecondTime = 0;

	dwTextAni = 0;
	dwTextAniTime = 0;
	dwAudienceAni = 0;
	dwAudienceAniTime = 0;
}

CBattleResult::~CBattleResult() {
	DeleteDC(hBackgroundDC);
	DeleteDC(hObjectDC);
	DeleteDC(hBomberDC);
}

void CBattleResult::Init(PLAYER winner) {
	bOn = true;
	battleStep = BATTLE_RESULT;

	this->winner = winner;

	if (winner == PLAYER_P1 || winner == PLAYER_P1AI) {
		n1PWinCount++;
	}
	else {
		n2PWinCount++;
	}

	dwSecond = 0;

	bomber[0] = CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[0].bomber;
	bomber[1] = CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[1].bomber;

	nAlpha = 255;
	bFadeIn = true;
	bFadeOut = false;
	BlendFunction.SourceConstantAlpha = nAlpha;

	CSoundManager::getInstance()->StopChBGM();
	CSoundManager::getInstance()->StartChBGM(BATTLE_RESULT_BGM);
}

void CBattleResult::SetAni() {
	if (dwCurTime - dwTextAniTime > 195) {
		dwTextAniTime = dwCurTime;

		dwTextAni += 640;

		if (dwTextAni == 1920) {
			dwTextAni = 0;
		}
	}

	if (dwCurTime - dwAudienceAniTime > 130) {
		dwAudienceAniTime = dwCurTime;

		dwAudienceAni += 1216;

		if (dwAudienceAni == 3648) {
			dwAudienceAni = 0;
		}
	}

	if (dwCurTime - dwBomberAniTime > 400) {
		dwBomberAniTime = dwCurTime;

		dwBomberAni += 128;

		if (dwBomberAni == 256) {
			dwBomberAni = 0;
		}
	}
}

BATTLE_STEP CBattleResult::Update() {
	dwCurTime = GetTickCount64();

	if (!bFadeIn) {
		SetAni();
	}

	if (dwCurTime - dwSecondTime > 1000) {
		dwSecondTime = dwCurTime;
		dwSecond++;
		if (dwSecond == 4) {
			bFadeOut = true;
		}
	}

	if (bFadeIn && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeIn();
	}

	if (bFadeOut && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeOut();

		if (!bFadeOut) {
			bOn = false;
			if (n1PWinCount < 2 && n2PWinCount < 2) {
				battleStep = BATTLE_PLAY;
			}
			else {
				CBattleManager::getInstance(hWnd, hBackbuffer)->gameStep = STEP_INTRO;
			}
		}
	}

	return battleStep;
}

void CBattleResult::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, 0, 0, SRCCOPY);

	BitBlt(hBackbuffer, 288, 104, 640, 92, hObjectDC, dwTextAni, RESULT_TEXT, SRCCOPY);
	BitBlt(hBackbuffer, 0, 768, WIN_RIGHT, 128, hObjectDC, dwAudienceAni, RESULT_AUDIENCE, SRCCOPY);

	// ¿Õ°ü
	for (int i = 0; i < n1PWinCount; i++) {
		BitBlt(hBackbuffer, 524 + (i * 128), 244, 40, 28, hObjectDC, 0, RESULT_CROWN, SRCCOPY);
	}

	for (int i = 0; i < n2PWinCount; i++) {
		BitBlt(hBackbuffer, 524 + (i * 128), 308, 40, 28, hObjectDC, 0, RESULT_CROWN, SRCCOPY);
	}

	if (winner == PLAYER_P1 || winner == PLAYER_P1AI) {
		BitBlt(hBackbuffer, 257, 544, 128, 120, hBomberDC, dwBomberAni, 120 * bomber[0], SRCCOPY);
		BitBlt(hBackbuffer, 448, 544, 128, 120, hBomberDC, 0, 120 * bomber[1], SRCCOPY);
	}
	else {
		BitBlt(hBackbuffer, 257, 544, 128, 120, hBomberDC, 0, 120 * bomber[0], SRCCOPY);
		BitBlt(hBackbuffer, 448, 544, 128, 120, hBomberDC, dwBomberAni, 120 * bomber[1], SRCCOPY);
	}

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}