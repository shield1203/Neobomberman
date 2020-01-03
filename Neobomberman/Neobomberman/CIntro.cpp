#include"CIntro.h"
#include"CSoundManager.h"

CIntro* CIntro::inst = nullptr;

CIntro* CIntro::getInstance(HWND hWnd, HDC hBackbuffer) {
	if (inst == nullptr) {
		inst = new CIntro(hWnd, hBackbuffer);
	}

	return inst;
}

CIntro::CIntro(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	hIntroDC = CreateBmpDC("Resource\\Intro\\Intro.bmp");
	hNumberDC = CreateBmpDC("Resource\\Intro\\Number.bmp");
	hTextDC = CreateBmpDC("Resource\\Intro\\Text.bmp");

	Init();
}

CIntro::~CIntro() {
	DeleteDC(hIntroDC);
	DeleteDC(hNumberDC);
	DeleteDC(hTextDC);
}

void CIntro::Init() {
	GameStep = STEP_INTRO;

	dwCurTime = 0;
	dwButtonTime = 0;
	dwTextTime = 0;
	dwCoin = 0;

	bCoin = false;

	CSoundManager::getInstance()->StopChBGM();
	CSoundManager::getInstance()->StartChBGM(INTRO_BGM);
}

void CIntro::PressKey() {
	if (GetKeyState(KEY_5) < 0 && dwCurTime - dwButtonTime > 150) { // 코인 투입!
		dwButtonTime = dwCurTime;

		if (dwCoin < 100) {
			dwCoin++;
			bCoin = true;
		}
		CSoundManager::getInstance()->StopChBGM();
		CSoundManager::getInstance()->StartChEffect(SOUND_COIN);
	}
	else if (GetKeyState(KEY_1) < 0 && dwCurTime - dwButtonTime > 150) {
		dwButtonTime = dwCurTime;

		if (dwCoin > 0) {
			GameStep = STEP_MODE_SELECT;
		}
	}
}

void CIntro::CheckCoin() {
	if (dwCurTime - dwTextTime > 200 && dwCoin == 0) {
		dwTextTime = dwCurTime;
		if (bCoin) {
			bCoin = false;
		}
		else {
			bCoin = true;
		}
	}
}

GAME_STEP CIntro::Update() {
	dwCurTime = GetTickCount64();
	CheckCoin();
	PressKey();

	return GameStep;
}

void CIntro::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hIntroDC, 0, 0, SRCCOPY);

	if (bCoin) {
		TransparentBlt(hBackbuffer, 448, 578, 415, 32, hTextDC, 0, 0, 415, 32, RGB(0, 248, 0)); // PUSH P1 START
	}

	if (dwCoin >= 2) {
		TransparentBlt(hBackbuffer, 870, 865, 224, 32, hTextDC, 0, 64, 224, 32, RGB(0, 248, 0)); 
	}
	else {
		TransparentBlt(hBackbuffer, 870, 865, 192, 32, hTextDC, 0, 32, 192, 32, RGB(0, 248, 0)); // 크래딧
	}

	TransparentBlt(hBackbuffer, 1114, 865, 28, 32, hNumberDC, (dwCoin / 10) * 28, 0, 28, 32, RGB(0, 248, 0)); 
	TransparentBlt(hBackbuffer, 1142, 865, 28, 32, hNumberDC, (dwCoin % 10) * 28, 0, 28, 32, RGB(0, 248, 0));
}