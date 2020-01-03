#include"CModeSelect.h"
#include"CSoundManager.h"

CModeSelect::CModeSelect(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer){
	hBackgroundStoryDC = CreateBmpDC("Resource\\ModeSelect\\BackgroundStory.bmp");
	hBackgroundBattleDC = CreateBmpDC("Resource\\ModeSelect\\BackgroundBattle.bmp");
	hSelectorDC = CreateBmpDC("Resource\\ModeSelect\\Selector.bmp");

	gameStep = STEP_MODE_SELECT;
	mode = SELECT_STORY;

	dwCurTime = 0;
	dwButtonTime = 0;
	dwMoveTime = 0;

	nAlpha = 255;
	bFadeIn = true;
	bFadeOut = false;
	BlendFunction.SourceConstantAlpha = nAlpha;

	CSoundManager::getInstance()->StartChBGM(SELECT_BGM);
}

CModeSelect::~CModeSelect() {
	DeleteDC(hBackgroundStoryDC);
	DeleteDC(hBackgroundBattleDC);
	DeleteDC(hSelectorDC);
}

void CModeSelect::CheckKey() {
	if ((GetKeyState(VK_UP) < 0) && dwCurTime - dwButtonTime > 150)
	{
		dwButtonTime = dwCurTime;
		selector.state = SELECTOR_MOVE;
		selector.way = WAY_UP;

		CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
	}
	else if ((GetKeyState(VK_DOWN) < 0) && dwCurTime - dwButtonTime > 150) {
		dwButtonTime = dwCurTime;
		selector.state = SELECTOR_MOVE;
		selector.way = WAY_DOWN;

		CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
	}
	else if ((GetKeyState(KEY_K) < 0) && dwCurTime - dwButtonTime > 150 && selector.way == WAY_STOP) {
		dwButtonTime = dwCurTime;
		selector.state = SELECTOR_SELECT;
		selector.dwSelectorAni = 0;

		CSoundManager::getInstance()->StartChEffect(SOUND_SELECT);
	}
}

void CModeSelect::SetAni() {
	if (selector.fYPos < 540) {
		mode = SELECT_STORY;
	}
	else {
		mode = SELECT_BATTLE;
	}
	
	switch (selector.state) {
		case SELECTOR_MOVE:
		{
			if (dwCurTime - selector.dwSelectorAniTime > 170 && selector.way != WAY_STOP) {
				selector.dwSelectorAniTime = dwCurTime;

				if (selector.dwSelectorAni < SELECTOR_MOVE_MAX) {
					selector.dwSelectorAni += 150;
				}
				else {
					selector.dwSelectorAni = 0;
				}
			}
			break;
		}
		case SELECTOR_SELECT:
		{
			if (dwCurTime - selector.dwSelectorAniTime > 50) {
				selector.dwSelectorAniTime = dwCurTime;

				if (selector.dwSelectorAni == SELECTOR_SELECT_MAX) {
					selector.dwSelectorAni = 0;
					selector.bSelect = true;
					selector.state = SELECTOR_IDLE;
				}
				else {
					selector.dwSelectorAni += 150;
				}
			}
			break;
		}
	}

	if (dwCurTime - selector.dwSelectAniTime > 37 && selector.bSelect) {
		selector.dwSelectAniTime = dwCurTime;
		if (selector.dwSelectAni != SELECTOR_SELECT_MAX) {
			selector.dwSelectAni += 200;

			if (selector.dwSelectAni == SELECTOR_SELECT_MAX) {
				bFadeOut = true;
			}
		}
	}
}

void CModeSelect::Move() {
	if (dwCurTime - dwMoveTime > 10) {
		dwMoveTime = dwCurTime;
		switch (selector.way)
		{
			case WAY_UP:
			{
				if (selector.fYPos > 380) {
					selector.fYPos -= 8;
				}
				else {
					selector.way = WAY_STOP;
					selector.fYPos = 380;
					selector.dwSelectorAni = 0;
				}
				break;
			}
			case WAY_DOWN:
			{
				if (selector.fYPos < 700) {
					selector.fYPos += 8;
				}
				else {
					selector.way = WAY_STOP;
					selector.fYPos = 700;
					selector.dwSelectorAni = 0;
				}
				break;
			}
		break;
		}
	}
}

GAME_STEP CModeSelect::Update() {
	dwCurTime = GetTickCount64();

	if (!selector.bSelect && selector.state != SELECTOR_SELECT && !bFadeIn && !bFadeOut) {
		CheckKey();
		Move();
	}

	if (bFadeIn && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeIn();
	}

	if (bFadeOut && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeOut();

		if (!bFadeOut) {
			if (mode == SELECT_BATTLE) {
				gameStep = STEP_BATTLE;
			}
			else if(mode == SELECT_STORY) {
				gameStep = STEP_STORY;
			}
		}
	}

	SetAni();

	return gameStep;
}

void CModeSelect::Render() {
	if (mode == SELECT_STORY) {
		BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundStoryDC, 0, 0, SRCCOPY); // 배경
	}
	else {
		BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundBattleDC, 0, 0, SRCCOPY); // 배경
	}

	TransparentBlt(hBackbuffer, selector.fXPos, selector.fYPos, 150, 150, hSelectorDC, 
		selector.dwSelectorAni, selector.state, 150, 150, RGB(0, 248, 0)); // 셀렉터(화이트봉)

	if (selector.bSelect) {
		if (mode == SELECT_STORY) {
			TransparentBlt(hBackbuffer, 180, 350, 200, 170, hSelectorDC, selector.dwSelectAni, 300, 200, 170, RGB(0, 248, 0)); // 선택시 그리는 원
		}
		else if (mode == SELECT_BATTLE) {
			TransparentBlt(hBackbuffer, 180, 680, 200, 170, hSelectorDC, selector.dwSelectAni, 300, 200, 170, RGB(0, 248, 0)); 
		}
	}

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}