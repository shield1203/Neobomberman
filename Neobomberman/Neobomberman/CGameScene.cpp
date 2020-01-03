#include"CGameScene.h"
#include"CSoundManager.h"

CGameScene::CGameScene(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	hSceneDC = CreateBmpDC("Resource\\GameScene\\Stage01.bmp");

	storyStep = STORY_SCENE;

	dwCurTime = 0;
	dwObjectTime = 0;
	dwSecond = 0;
	dwSecondTime = 0;
	dwScene = 0;

	fObjectXPos = 412;
	fObjectYPos = 473;

	nAlpha = 255;
	bFadeIn = true;

	CSoundManager::getInstance()->StopChBGM();
	CSoundManager::getInstance()->StartChBGM(GAME_SCENE_BGM);
}

CGameScene::~CGameScene() {
	DeleteDC(hSceneDC);
}

void CGameScene::CheckKey() {
	if (GetKeyState(KEY_K) < 0) {
		bFadeOut = true;
	}
}

void CGameScene::ObjectMove() {
	if (dwCurTime - dwObjectTime > 5) {
		dwObjectTime = dwCurTime;

		fObjectYPos -= 2;

		if (fObjectYPos == 161) {
			dwScene = 305;
		}
	}
}

STORY_STEP CGameScene::Update() {
	dwCurTime = GetTickCount64();

	if (dwScene != 305 && !bFadeIn && !bFadeOut) {
		CheckKey();
		ObjectMove();
	}
	else {
		if (dwCurTime - dwSecondTime > 1000) {
			dwSecondTime = dwCurTime;
			dwSecond++;
		}

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
			storyStep = STORY_PLAY;
		}
	}

	return storyStep;
}

void CGameScene::Render() {
	PatBlt(hBackbuffer, 0, 0, 1700, 800, BLACKNESS);
	StretchBlt(hBackbuffer, 0, 192, WIN_RIGHT, 280, hSceneDC, dwScene, 0, 304, 70, SRCCOPY);

	TransparentBlt(hBackbuffer, fObjectXPos, fObjectYPos, 400, 308, hSceneDC, 685, 14, 100, 77, RGB(0, 0, 255)); // 오브젝트

	StretchBlt(hBackbuffer, 0, 472, WIN_RIGHT, 296, hSceneDC, dwScene, 70, 304, 74, SRCCOPY);

	BitBlt(hBackbuffer, 0, 768, WIN_RIGHT, 100, hFadeDC, 0, 0, SRCCOPY); // 삐져나오는 오브젝트 가리기용

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}