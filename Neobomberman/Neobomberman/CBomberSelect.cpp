#include"CBomberSelect.h"
#include"CSoundManager.h"

CBomberSelect::CBomberSelect(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer){
	hBackgroundDC = CreateBmpDC("Resource\\BomberSelect\\Background.bmp");
	hSelectorDC = CreateBmpDC("Resource\\BomberSelect\\Selector.bmp");
	hBomberNameDC = CreateBmpDC("Resource\\BomberSelect\\Name.bmp");
	hBombermansDC = CreateBmpDC("Resource\\BomberSelect\\Bombermans.bmp");	

	Player[0].player = PLAYER_P1;
	Player[1].player = PLAYER_P2AI;

	Init();
}

CBomberSelect::~CBomberSelect() {
	DeleteDC(hBackgroundDC);
	DeleteDC(hSelectorDC);
	DeleteDC(hBomberNameDC);
	DeleteDC(hBombermansDC);
}

void CBomberSelect::Init() {
	bOn = true;

	for (int i = 0; i < 2; i++) {
		Player[i].bPSelect = false;
		Player[i].bReady = false;

		Player[i].dwBombermanAni = 0;

		Player[i].nPRow = 0;
		Player[i].nPColumn = 0;

		Player[i].key = VK_RIGHT;
	}

	dwCurTime = 0;

	nAlpha = 255;
	bFadeIn = true;
	bFadeOut = false;
	BlendFunction.SourceConstantAlpha = nAlpha;
}

//void CBomberSelect::Init(asdasd) {
//
//}

void CBomberSelect::CheckKey() {
	if (Player[0].player == PLAYER_P1AI) {
		if ((GetKeyState(KEY_1) < 0) && dwCurTime - Player[0].dwButtonTime > 150) { // 2P »ý¼º
			Player[0].dwButtonTime = dwCurTime;

			Player[0].player = PLAYER_P1;

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
	}
	else if (!Player[0].bPSelect) {  // 1P
		if ((GetKeyState(VK_RIGHT) < 0) && dwCurTime - Player[0].dwButtonTime > 150) {
			Player[0].dwButtonTime = dwCurTime;
			Player[0].key = VK_RIGHT;
			Player[0].dwBombermanAni = 0;

			Player[0].nPColumn++;
			Player[0].nPColumn %= 6;

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(VK_LEFT) < 0) && dwCurTime - Player[0].dwButtonTime > 150) {
			Player[0].dwButtonTime = dwCurTime;
			Player[0].key = VK_LEFT;
			Player[0].dwBombermanAni = 0;

			if (Player[0].nPColumn - 1 > -1) {
				Player[0].nPColumn--;
			}
			else {
				Player[0].nPColumn = 5;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(VK_UP) < 0) && dwCurTime - Player[0].dwButtonTime > 150) {
			Player[0].dwButtonTime = dwCurTime;
			Player[0].key = VK_UP;
			Player[0].dwBombermanAni = 0;

			if (Player[0].nPRow == 0) {
				Player[0].nPRow = 1;
			}
			else {
				Player[0].nPRow = 0;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(VK_DOWN) < 0) && dwCurTime - Player[0].dwButtonTime > 150) {
			Player[0].dwButtonTime = dwCurTime;
			Player[0].key = VK_DOWN;
			Player[0].dwBombermanAni = 0;

			if (Player[0].nPRow == 0) {
				Player[0].nPRow = 1;
			}
			else {
				Player[0].nPRow = 0;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(KEY_K) < 0) && dwCurTime - Player[0].dwButtonTime > 150) {
			Player[0].dwButtonTime = dwCurTime;
			Player[0].key = KEY_K;
			Player[0].bPSelect = true;
			Player[0].dwBombermanAni = 96;

			CSoundManager::getInstance()->StartChEffect(SOUND_SELECT);
		}
	}

	if (Player[1].player == PLAYER_P2AI) { 
		if ((GetKeyState(KEY_2) < 0) && dwCurTime - Player[1].dwButtonTime > 150) { // 2P »ý¼º
			Player[1].dwButtonTime = dwCurTime;

			Player[1].player = PLAYER_P2;

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
	}
	else if(!Player[1].bPSelect) {
		if ((GetKeyState(KEY_G) < 0) && dwCurTime - Player[1].dwButtonTime > 150) {
			Player[1].dwButtonTime = dwCurTime;
			Player[1].key = KEY_G;
			Player[1].dwBombermanAni = 0;
			
			Player[1].nPColumn++;
			Player[1].nPColumn %= 6;

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(KEY_D) < 0) && dwCurTime - Player[1].dwButtonTime > 150) {
			Player[1].dwButtonTime = dwCurTime;
			Player[1].key = KEY_D;
			Player[1].dwBombermanAni = 0;

			if (Player[1].nPColumn - 1 > -1) {
				Player[1].nPColumn--;
			}
			else {
				Player[1].nPColumn = 5;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(KEY_R) < 0) && dwCurTime - Player[1].dwButtonTime > 150) {
			Player[1].dwButtonTime = dwCurTime;
			Player[1].key = KEY_R;
			Player[1].dwBombermanAni = 0;
			
			if (Player[1].nPRow == 0) {
				Player[1].nPRow = 1;
			}
			else {
				Player[1].nPRow = 0;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(KEY_F) < 0) && dwCurTime - Player[1].dwButtonTime > 150) {
			Player[1].dwButtonTime = dwCurTime;
			Player[1].key = KEY_F;
			Player[1].dwBombermanAni = 0;

			if (Player[1].nPRow == 0) {
				Player[1].nPRow = 1;
			}
			else {
				Player[1].nPRow = 0;
			}

			CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
		}
		else if ((GetKeyState(KEY_A) < 0) && dwCurTime - Player[1].dwButtonTime > 150) {
			Player[1].dwButtonTime = dwCurTime;
			Player[1].key = KEY_A;
			Player[1].bPSelect = true;
			Player[1].dwBombermanAni = 96;

			CSoundManager::getInstance()->StartChEffect(SOUND_SELECT);
		}
	}
	
}

void CBomberSelect::CheckSelect() {
	if (Player[0].nPColumn == Player[1].nPColumn && Player[0].nPRow == Player[1].nPRow) { // ¼±ÅÃµÈ º½¹ö°¡ °°À»¶§ ¹Ð¾î³»±â
		if (Player[0].bPSelect) {
			switch (Player[1].key)
			{
				case KEY_G: { // ¿À¸¥ÂÊ 
					Player[1].nPColumn++;
					Player[1].nPColumn %= 6;
					break;
				}
				case KEY_D: {
					Player[1].nPColumn--;
					if (Player[1].nPColumn < 0) {
						Player[1].nPColumn = 5;
					}
					break;
				}
				default: {
					if (Player[1].nPRow == 0) {
						Player[1].nPRow = 1;
					}
					else {
						Player[1].nPRow = 0;
					}
					break;
				}
			}
		}
		else if (Player[1].bPSelect) {
			switch (Player[0].key)
			{
				case VK_RIGHT: { // ¿À¸¥ÂÊ 
					Player[0].nPColumn++;
					Player[0].nPColumn %= 6;
					break;
				}
				case VK_LEFT: {
					Player[0].nPColumn--;
					if (Player[0].nPColumn < 0) {
						Player[0].nPColumn = 5;
					}
					break;
				}
				default: {
					if (Player[0].nPRow == 0) {
						Player[0].nPRow = 1;
					}
					else {
						Player[0].nPRow = 0;
					}
					break;
				}
			}
		}
	}
}

void CBomberSelect::SetAni() {
	if (Player[0].player == PLAYER_P1) {
		if (dwCurTime - Player[0].dwSelectorAniTime > 310) { // 1P ¼¿·ºÅÍ
			Player[0].dwSelectorAniTime = dwCurTime;

			Player[0].dwSelectorAni += 276;

			if (Player[0].dwSelectorAni >= 1104) {
				Player[0].dwSelectorAni = 0;
			}
		}
	}

	if (Player[1].player == PLAYER_P2) { // 2P ¼¿·ºÅÍ
		if (dwCurTime - Player[1].dwSelectorAniTime > 310) {
			Player[1].dwSelectorAniTime = dwCurTime;

			Player[1].dwSelectorAni += 276;

			if (Player[1].dwSelectorAni >= 1104) {
				Player[1].dwSelectorAni = 0;
			}
		}
	}

	//-----------------------º½¹ö¸Ç 1P
	if (Player[0].bPSelect) {
		if (dwCurTime - Player[0].dwBombermanAniTime > 50) {
			Player[0].dwBombermanAniTime = dwCurTime;
			if (!Player[0].bReady) {
				Player[0].dwBombermanAni += 32;
				if (Player[0].dwBombermanAni == 352) {
					Player[0].bReady = true;
				}
			}
			else {
				if (Player[0].dwBombermanAni != 96) {
					Player[0].dwBombermanAni -= 32;
				}
			}
		}
	}
	else {
		if (dwCurTime - Player[0].dwBombermanAniTime > 100 && Player[0].dwBombermanAni < 64) {
			Player[0].dwBombermanAniTime = dwCurTime;
			Player[0].dwBombermanAni += 32;
		}
	}

	//----------------------º½¹ö¸Ç 2P
	if (Player[1].bPSelect) {
		if (dwCurTime - Player[1].dwBombermanAniTime > 50) {
			Player[1].dwBombermanAniTime = dwCurTime;
			if (!Player[1].bReady) {
				Player[1].dwBombermanAni += 32;
				if (Player[1].dwBombermanAni == 352) {
					Player[1].bReady = true;
				}
			}
			else {
				if (Player[1].dwBombermanAni != 96) {
					Player[1].dwBombermanAni -= 32;
				}
			}
		}
	}
	else {
		if (dwCurTime - Player[1].dwBombermanAniTime > 100 && Player[1].dwBombermanAni < 64) {
			Player[1].dwBombermanAniTime = dwCurTime;
			Player[1].dwBombermanAni += 32;
		}
	}
}

void CBomberSelect::Update() {
	dwCurTime = GetTickCount64();

	if (!bFadeIn && !bFadeOut) {
		CheckKey();
		if (Player[0].player == PLAYER_P1 && Player[1].player == PLAYER_P2) { 
			CheckSelect();
		}

		if (Player[0].player == PLAYER_P1 && Player[1].player == PLAYER_P2) {
			if (Player[0].bReady && Player[0].dwBombermanAni == 96 && Player[1].bReady && Player[1].dwBombermanAni == 96) {
				Player[0].bomber = (BOMBER)(Player[0].nPRow * 6 + Player[0].nPColumn);
				Player[1].bomber = (BOMBER)(Player[1].nPRow * 6 + Player[1].nPColumn);
				bFadeOut = true;
			}
		}
		else if (Player[1].player == PLAYER_P2) {
			if (Player[1].bReady && Player[1].dwBombermanAni == 96) {
				Player[1].bomber = (BOMBER)(Player[1].nPRow * 6 + Player[1].nPColumn);
				bFadeOut = true;
			}
		}
		else if (Player[0].player == PLAYER_P1){
			if (Player[0].bReady && Player[0].dwBombermanAni == 96) {
				Player[0].bomber = (BOMBER)(Player[0].nPRow * 6 + Player[0].nPColumn);
				bFadeOut = true;
			}
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
			CBattleManager::getInstance(hWnd, hBackbuffer)->battleStep = BATTLE_PLAY;
		}
	}

	SetAni();
}

void CBomberSelect::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, 0, 0, SRCCOPY);

	if (Player[0].player == PLAYER_P1) {
		TransparentBlt(hBackbuffer, 44 + (192 * Player[0].nPColumn), 197 + (192 * Player[0].nPRow), 276, 304, hSelectorDC, Player[0].dwSelectorAni, 0, 276, 304, RGB(0, 248, 0));
		TransparentBlt(hBackbuffer, 645, 730, 288, 131, hBomberNameDC, 288 * (Player[0].nPColumn + (Player[0].nPRow * 6)), 0, 288, 126, RGB(0, 248, 0)); // º½¹ö¸Ç ÀÌ¸§
	}

	if (Player[1].player == PLAYER_P2) {
		TransparentBlt(hBackbuffer, -63 + (192 * Player[1].nPColumn), 197 + (192 * Player[1].nPRow), 276, 304, hSelectorDC, Player[1].dwSelectorAni, 304, 276, 304, RGB(0, 248, 0)); // ¼¿·ºÅÍ
		TransparentBlt(hBackbuffer, 65, 730, 288, 131, hBomberNameDC, 288 * (Player[1].nPColumn + (Player[1].nPRow * 6)), 0, 288, 126, RGB(0, 248, 0)); // º½¹ö¸Ç ÀÌ¸§
	}

	for (int i = 0; i < 6; i++) {
		if (Player[0].player == PLAYER_P1 && Player[0].nPColumn + (Player[0].nPRow * 6) == i) {
			if (Player[0].bReady) {
				TransparentBlt(hBackbuffer, 461, 778, 100, 100, hBombermansDC, Player[0].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
				TransparentBlt(hBackbuffer, 461, 728, 100, 100, hBombermansDC, Player[0].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
			}
			else {
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 405, 100, 100, hBombermansDC, Player[0].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 355, 100, 100, hBombermansDC, Player[0].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
			}
		}
		else if (Player[1].player == PLAYER_P2 && Player[1].nPColumn + (Player[1].nPRow * 6) == i) {
			if (Player[1].bReady) {
				TransparentBlt(hBackbuffer, 1037, 778, 100, 100, hBombermansDC, Player[1].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); 
				TransparentBlt(hBackbuffer, 1037, 728, 100, 100, hBombermansDC, Player[1].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255)); 
			}
			else {
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 405, 100, 100, hBombermansDC, Player[1].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 355, 100, 100, hBombermansDC, Player[1].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255));
			}
		}
		else {
			TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 405 + ((i / 6) * 100), 100, 100, hBombermansDC, 0, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
			TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 355 + ((i / 6) * 100), 100, 100, hBombermansDC, 0, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
		}
	}

	for (int i = 6; i < 12; i++) {
		if (Player[0].player == PLAYER_P1 && Player[0].nPColumn + (Player[0].nPRow * 6) == i) {
			if (Player[0].bReady) {
				TransparentBlt(hBackbuffer, 461, 778, 100, 100, hBombermansDC, Player[0].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
				TransparentBlt(hBackbuffer, 461, 728, 100, 100, hBombermansDC, Player[0].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
			}
			else {
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 597, 100, 100, hBombermansDC, Player[0].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 547, 100, 100, hBombermansDC, Player[0].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
			}
		}
		else if (Player[1].player == PLAYER_P2 && Player[1].nPColumn + (Player[1].nPRow * 6) == i) {
			if (Player[1].bReady) {
				TransparentBlt(hBackbuffer, 1037, 778, 100, 100, hBombermansDC, Player[1].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255));
				TransparentBlt(hBackbuffer, 1037, 728, 100, 100, hBombermansDC, Player[1].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255));
			}
			else {
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 597, 100, 100, hBombermansDC, Player[1].dwBombermanAni, (64 * i) + 32, 32, 32, RGB(255, 0, 255));
				TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 547, 100, 100, hBombermansDC, Player[1].dwBombermanAni, 64 * i, 32, 32, RGB(255, 0, 255));
			}
		}
		else {
			TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 597, 100, 100, hBombermansDC, 0, (64 * i) + 32, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸öÅë
			TransparentBlt(hBackbuffer, 76 + ((i % 6) * 192), 547, 100, 100, hBombermansDC, 0, 64 * i, 32, 32, RGB(255, 0, 255)); // º½¹ö¸Ç ¸Ó°¡·ò
		}
	}

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}