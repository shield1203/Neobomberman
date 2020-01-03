#include"CStory.h"
#include"CSoundManager.h"

CStory::CStory(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	h1PDC = CreateBmpDC("Resource\\Story\\White.bmp");
	hClearDC = CreateBmpDC("Resource\\Story\\Clear.bmp");
	hEnemyDC = CreateBmpDC("Resource\\Story\\Enemies.bmp");
	hInterfaceDC = CreateBmpDC("Resource\\Story\\Interface.bmp");
	hNumDC = CreateBmpDC("Resource\\Story\\Num.bmp");
	hItemDC = CreateBmpDC("Resource\\Battle\\Item.bmp");
	hBombDC = CreateBmpDC("Resource\\Battle\\Bomb.bmp");

	MapDCLoad();

	storyStep = STORY_PLAY;

	dwCurTime = 0;
	dwSecond = 180;
	dwSecondTime = 0;

	nPushStart = 61;
	dwPushStartTime = 0;

	bomber.InitBomber(PLAYER_P1, WHITEBOMBER, 4);

	nScore = 0;

	LoadMapData("Stage1.xml");

	Enemy[0] = new CEnemy(SeekerBalloon, mapBlock[137].rtBlock, 137);
	Enemy[1] = new CEnemy(Matango, mapBlock[135].rtBlock, 135);
	Enemy[2] = new CEnemy(GingerBreadman, mapBlock[123].rtBlock, 123);

	bClear = false;
	bFail = false;
	dwClearAni = 0;
	dwResultTime = 0;
	nResultCount = 0;

	nAlpha = 255;
	bFadeIn = true;

	CSoundManager::getInstance()->StopChBGM();
	CSoundManager::getInstance()->StartChBGM(STAGE1_BGM);
}

CStory::~CStory() {
	DeleteDC(h1PDC);
	DeleteDC(hClearDC);
	DeleteDC(hEnemyDC);
	DeleteDC(hInterfaceDC);
	DeleteDC(hNumDC);
	DeleteDC(hItemDC);
	DeleteDC(hBombDC);
	DeleteDC(hBackgroundDC);

	for (int i = 0; i < 22; i++) {
		DeleteDC(hTyleDC[i]);
	}

	for (int i = 0; i < 7; i++) {
		DeleteDC(hObjectDC[i]);
	}

	for (int i = 0; i < 3; i++) {
		DeleteDC(hBlockDC[i]);
	}

	for (int i = 0; i < bomb.size(); i++) {
		CBomb* bombs = bomb[i];
		delete bombs;
	}
	bomb.clear();

	for (int i = 0; i < item.size(); i++) {
		CItem* items = item[i];
		delete items;
	}
	item.clear();
}

void CStory::LoadMapData(string strMapFilrName) {
	// Map Data 
	TiXmlDocument doc;
	doc.LoadFile(strMapFilrName.c_str());

	TiXmlElement* pRoot = doc.FirstChildElement("Map");
	if (!pRoot) return;

	TiXmlElement*  pElem = pRoot->FirstChildElement("MapBlock");
	TiXmlAttribute* pAttrib = nullptr;

	RECT rt = { 192, 128, 256, 192 };
	for (int i = 0; i < 143; i++) {
		mapBlock[i].rtBlock.left = rt.left;
		mapBlock[i].rtBlock.right = rt.right;
		mapBlock[i].rtBlock.top = rt.top;
		mapBlock[i].rtBlock.bottom = rt.bottom;

		pAttrib = pElem->FirstAttribute();
		mapBlock[i].Tyle = (TYLE)pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		mapBlock[i].Object = (OBJECT)pAttrib->IntValue();
		pAttrib = pAttrib->Next();

		mapBlock[i].Block = (BLOCK)pAttrib->IntValue();

		if (mapBlock[i].Object != NONE_OBJECT || mapBlock[i].Block != NONE_BLOCK) {
			mapBlock[i].bOn = true;
		}

		pElem = pElem->NextSiblingElement();

		if (rt.left == 960) {
			rt.left = 192;
			rt.right = 256;
			rt.top += 64;
			rt.bottom += 64;
		}
		else {
			rt.left += 64;
			rt.right += 64;
		}
	}
}

void CStory::MapDCLoad() {
	hBackgroundDC = CreateBmpDC("Resource\\Battle\\Background04.bmp");

	for (int i = 0; i < 23; i++) {
		string filename = "Resource\\Battle\\Tyle.bmp";

		if (i + 1 < 10) {
			string num = "0";
			num.append(to_string(i + 1));
			filename.insert(20, num);
		}
		else {
			filename.insert(20, to_string(i + 1));
		}

		hTyleDC[i] = CreateBmpDC(filename.c_str());
	}

	for (int i = 0; i < 8; i++) {
		string filename = "Resource\\Battle\\Object0.bmp";
		filename.insert(23, to_string(i + 1));
		hObjectDC[i] = CreateBmpDC(filename.c_str());
	}

	for (int i = 0; i < 4; i++) {
		string filename = "Resource\\Battle\\Block0.bmp";
		filename.insert(22, to_string(i + 1));
		hBlockDC[i] = CreateBmpDC(filename.c_str());
	}
}

void CStory::BomberControl() {
	if (GetAsyncKeyState(KEY_K) < 0 && dwCurTime - bomber.dwButtonTime > 190) { // 폭탄 설치
		bomber.state = P_STATE_MOVE;
		bomber.dwButtonTime = dwCurTime;

		bool bCheck = true;
		if (mapBlock[bomber.dwLocation].bOn) {
			bCheck = false;
		}

		int nBombCount = 0;
		for (int i = 0; i < bomb.size(); i++) {
			if (bomb[i]->player == PLAYER_P1) {
				nBombCount++;
			}
		}

		if (bCheck && bomber.dwMaxBombCount > nBombCount) {
			bool bCheck = false;
			for (int i = 0; i < bomb.size(); i++) {
				if (!bomb[i]->bOn) {
					bomb[i]->SetBomb(bomber.player, bomber.dwLocation, bomber.dwBombRange);
					bCheck = true;
					break;
				}
			}

			if (!bCheck) {
				CBomb* addBomb = new CBomb();
				addBomb->SetBomb(bomber.player, bomber.dwLocation, bomber.dwBombRange);
				bomb.push_back(addBomb);
			}
			mapBlock[bomber.dwLocation].bOn = true;

			CSoundManager::getInstance()->StartSubChEffect(SOUND_SET_BOMB);
		}
	}
	else if (GetAsyncKeyState(VK_UP) < 0) {
		bomber.state = P_STATE_MOVE;
		bomber.direction = PLAYER_UP;
		bomber.SetAni(dwCurTime);

		if (bomber.fYPos - bomber.fSpeed > 74) {
			if (bomber.dwLocation < 13) {
				bomber.fYPos -= bomber.fSpeed;
			}
			else {
				if (!mapBlock[bomber.dwLocation - 13].bOn) {
					if (mapBlock[bomber.dwLocation - 13].rtBlock.left + 32 == bomber.pLocation.x) {
						bomber.fYPos -= bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation - 13].rtBlock.left + 32 > bomber.pLocation.x) {
						bomber.fXPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation - 13].rtBlock.left + 32 < bomber.pLocation.x) {
						bomber.fXPos -= bomber.fSpeed;
					}
				}
				else {
					if (mapBlock[bomber.dwLocation].rtBlock.bottom - 32 < bomber.pLocation.y) {
						bomber.fYPos -= bomber.fSpeed;
					}
				}
			}
		}
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0) {
		bomber.state = P_STATE_MOVE;
		bomber.direction = PLAYER_DOWN;
		bomber.SetAni(dwCurTime);

		if (bomber.fYPos + bomber.fSpeed < 716) {
			if (bomber.dwLocation > 129) {
				bomber.fYPos += bomber.fSpeed;
			}
			else {
				if (!mapBlock[bomber.dwLocation + 13].bOn) {
					if (mapBlock[bomber.dwLocation + 13].rtBlock.left + 32 == bomber.pLocation.x) {
						bomber.fYPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation + 13].rtBlock.left + 32 > bomber.pLocation.x) {
						bomber.fXPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation + 13].rtBlock.left + 32 < bomber.pLocation.x) {
						bomber.fXPos -= bomber.fSpeed;
					}
				}
				else {
					if (mapBlock[bomber.dwLocation].rtBlock.bottom - 32 > bomber.pLocation.y) {
						bomber.fYPos += bomber.fSpeed;
					}
				}
			}
		}
	}
	else if (GetAsyncKeyState(VK_LEFT) < 0) {
		bomber.state = P_STATE_MOVE;
		bomber.direction = PLAYER_LEFT;
		bomber.SetAni(dwCurTime);

		if (bomber.fXPos - bomber.fSpeed > 163) {
			if (bomber.dwLocation % 13 == 0) {
				bomber.fXPos -= bomber.fSpeed;
			}
			else {
				if (!mapBlock[bomber.dwLocation - 1].bOn) {
					if (mapBlock[bomber.dwLocation - 1].rtBlock.top + 32 == bomber.pLocation.y) {
						bomber.fXPos -= bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation - 1].rtBlock.top + 32 > bomber.pLocation.y) {
						bomber.fYPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation - 1].rtBlock.top + 32 < bomber.pLocation.y) {
						bomber.fYPos -= bomber.fSpeed;
					}
				}
				else {
					if (mapBlock[bomber.dwLocation].rtBlock.left + 32 < bomber.pLocation.x) {
						bomber.fXPos -= bomber.fSpeed;
					}
				}
			}
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0) {
		bomber.state = P_STATE_MOVE;
		bomber.direction = PLAYER_RIGHT;
		bomber.SetAni(dwCurTime);

		if (bomber.fXPos + bomber.fSpeed < 933) {
			if (bomber.dwLocation % 13 == 12) {
				bomber.fXPos += bomber.fSpeed;
			}
			else {
				if (!mapBlock[bomber.dwLocation + 1].bOn) {
					if (mapBlock[bomber.dwLocation + 1].rtBlock.top + 32 == bomber.pLocation.y) {
						bomber.fXPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation + 1].rtBlock.top + 32 > bomber.pLocation.y) {
						bomber.fYPos += bomber.fSpeed;
					}
					else if (mapBlock[bomber.dwLocation + 1].rtBlock.top + 32 < bomber.pLocation.y) {
						bomber.fYPos -= bomber.fSpeed;
					}
				}
				else {
					if (mapBlock[bomber.dwLocation].rtBlock.left + 32 > bomber.pLocation.x) {
						bomber.fXPos += bomber.fSpeed;
					}
				}
			}
		}
	}
	else {
		bomber.dwAni = 0;
	}

	bomber.SetRECT();
	LocationCheck();
}

void CStory::LocationCheck() {
	for (int i = 0; i < 143; i++) {
		if (bomber.pLocation.x >= mapBlock[i].rtBlock.left && bomber.pLocation.x <= mapBlock[i].rtBlock.right
			&& bomber.pLocation.y >= mapBlock[i].rtBlock.top && bomber.pLocation.y <= mapBlock[i].rtBlock.bottom) {
			bomber.dwLocation = i;
		}
	}
}

void CStory::SetBlockAni() {
	for (int i = 0; i < 143; i++) {
		if (mapBlock[i].bMelting) {
			switch (mapBlock[i].Block) {
			case BLOCK01: {
				if (dwCurTime - mapBlock[i].dwAniTime > 85) { // 680
					mapBlock[i].dwAniTime = dwCurTime;

					mapBlock[i].dwAni += 64;

					if (mapBlock[i].dwAni == 512) {
						mapBlock[i].dwAni = 0;
						mapBlock[i].Block = NONE_BLOCK;
						mapBlock[i].bMelting = false;
						mapBlock[i].bOn = false;
					}
				}
				break;
			}
			case BLOCK02: {
				if (dwCurTime - mapBlock[i].dwAniTime > 46) {
					mapBlock[i].dwAniTime = dwCurTime;

					mapBlock[i].dwAni += 64;

					if (mapBlock[i].dwAni == 960) {
						mapBlock[i].dwAni = 0;
						mapBlock[i].Block = NONE_BLOCK;
						mapBlock[i].bMelting = false;
						mapBlock[i].bOn = false;
					}
				}
				break;
			}
			case BLOCK03: {
				if (dwCurTime - mapBlock[i].dwAniTime > 68) {
					mapBlock[i].dwAniTime = dwCurTime;

					mapBlock[i].dwAni += 64;

					if (mapBlock[i].dwAni == 640) {
						mapBlock[i].dwAni = 0;
						mapBlock[i].Block = NONE_BLOCK;
						mapBlock[i].bMelting = false;
						mapBlock[i].bOn = false;
					}
				}
				break;
			}
			}

			if (!mapBlock[i].bOn) {
				DWORD dwItem = (GetTickCount64() + i) % 7; // 확률적으로 아이템이 생성되어야함

				if (dwItem < 2) {
					bool bCheck = true;
					ITEM tem = (ITEM)(dwItem * 92);

					for (int j = 0; j < item.size(); j++) {
						if (!item[j]->bOn) {
							bCheck = false;
							item[j]->Init(tem, i, mapBlock[i].rtBlock);
							break;
						}
					}

					if (bCheck) {
						CItem* addItem = new CItem();
						addItem->Init(tem, i, mapBlock[i].rtBlock);
						item.push_back(addItem);
					}
				}
			}
		}
	}
}

void CStory::SetClearAni() {
	if (dwCurTime - dwResultTime > 200) {
		dwResultTime = dwCurTime;

		dwClearAni += 702;

		if (dwClearAni == 4212) {
			dwClearAni = 0;
			nResultCount++;
		}
	}
}

void CStory::CheckCollision() {
	RECT rtCollision;
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i]->bExplosion) {
			if (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &bomber.rtCollision) ||
				IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &bomber.rtCollision)) {
				bomber.state = P_STATE_DIE;
				bomber.direction = PLAYER_DIE;
				bomber.dwAni = 0;

				dwDieTime = GetTickCount64();

				break;
			}

			for (int j = 0; j < item.size(); j++) {
				if (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &item[j]->rtCollision) ||
					IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &item[j]->rtCollision)) {
					item[j]->bOn = false;
				}
			}

			for (int j = 0; j < 3; j++) {
				if (Enemy[j]->nLife > 0 && (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &Enemy[j]->rtCollision) ||
					IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &Enemy[j]->rtCollision))) {
					nScore += 100 * j;
					Enemy[j]->nLife--;
				}
			}
		}
	}

	for (int i = 0; i < item.size(); i++) {
		if (item[i]->bOn) {
			if (IntersectRect(&rtCollision, &item[i]->rtCollision, &bomber.rtCollision)) {
				item[i]->bOn = false;
				switch (item[i]->kind)
				{
				case FIRE: {
					bomber.dwBombRange += 1;
					break;
				}
				case BOMB: {
					bomber.dwMaxBombCount += 1;
					break;
				}
				}
				CSoundManager::getInstance()->StartChEffect(SOUND_ITEM);
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if (Enemy[i]->nLife > 0 && IntersectRect(&rtCollision, &bomber.rtCollision, &Enemy[i]->rtCollision)) {
			bomber.state = P_STATE_DIE;
			bomber.direction = PLAYER_DIE;
			bomber.dwAni = 0;

			dwDieTime = GetTickCount64();
		}
	}
}

void CStory::CheckBombRange(int nBombIndex) {
	int nUP = 0;
	for (int i = 1; i <= bomb[nBombIndex]->dwRange; i++) { // UP
		if ((int)bomb[nBombIndex]->dwLocation - (13 * i) < 0 || bomb[nBombIndex]->dwLocation < 13) {
			break;
		}
		else if (mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].bOn) {
			if (mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].Block == NONE_BLOCK && mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].Block == NONE_OBJECT) {
				for (int j = 0; j < bomb.size(); j++) {
					if (bomb[nBombIndex]->dwLocation - 13 * i == bomb[j]->dwLocation) { // 연쇄폭팔
						bomb[j]->bExplosion = true;
						bomb[j]->dwAni = 0;
						bomb[j]->dwAniTime = 0;
						mapBlock[bomb[j]->dwLocation].bOn = false;
						CheckBombRange(j);
						break;
					}
				}
			}
			else if (mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].Block != NONE_BLOCK) {
				mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].bMelting = true;
			}
			break;
		}
		nUP++;
	}

	int nDOWN = 0;
	for (int i = 1; i <= bomb[nBombIndex]->dwRange; i++) { // DOWN
		if (bomb[nBombIndex]->dwLocation + 13 * i > 143) {
			break;
		}
		else if (mapBlock[bomb[nBombIndex]->dwLocation + 13 * i].bOn) {
			if (mapBlock[bomb[nBombIndex]->dwLocation + 13 * i].Block == NONE_BLOCK && mapBlock[bomb[nBombIndex]->dwLocation + 13 * i].Block == NONE_OBJECT) {
				for (int j = 0; j < bomb.size(); j++) {
					if (bomb[nBombIndex]->dwLocation + 13 * i == bomb[j]->dwLocation) {
						bomb[j]->bExplosion = true;
						bomb[j]->dwAni = 0;
						bomb[j]->dwAniTime = 0;
						mapBlock[bomb[j]->dwLocation].bOn = false;
						CheckBombRange(j);
						break;
					}
				}
			}
			else if (mapBlock[bomb[nBombIndex]->dwLocation + 13 * i].Block != NONE_BLOCK) {
				mapBlock[bomb[nBombIndex]->dwLocation + 13 * i].bMelting = true;
			}

			break;
		}
		nDOWN++;
	}

	int nLEFT = 0;
	for (int i = 1; i <= bomb[nBombIndex]->dwRange; i++) { // LEFT
		if (bomb[nBombIndex]->dwLocation - i == -1) {
			break;
		}
		else if ((bomb[nBombIndex]->dwLocation - i) % 13 == 12) {
			break;
		}
		else if (mapBlock[bomb[nBombIndex]->dwLocation - i].bOn) {
			if (mapBlock[bomb[nBombIndex]->dwLocation - i].Block == NONE_BLOCK && mapBlock[bomb[nBombIndex]->dwLocation - i].Block == NONE_OBJECT) {
				for (int j = 0; j < bomb.size(); j++) {
					if (bomb[nBombIndex]->dwLocation - i == bomb[j]->dwLocation) {
						bomb[j]->bExplosion = true;
						bomb[j]->dwAni = 0;
						bomb[j]->dwAniTime = 0;
						mapBlock[bomb[j]->dwLocation].bOn = false;
						CheckBombRange(j);
						break;
					}
				}
			}
			else if (mapBlock[bomb[nBombIndex]->dwLocation - i].Block != NONE_BLOCK) {
				mapBlock[bomb[nBombIndex]->dwLocation - i].bMelting = true;
			}

			break;
		}
		nLEFT++;
	}

	int nRIGHT = 0;
	for (int i = 1; i <= bomb[nBombIndex]->dwRange; i++) { // RIGHT
		if ((bomb[nBombIndex]->dwLocation + i) % 13 == 0) {
			break;
		}
		else if (mapBlock[bomb[nBombIndex]->dwLocation + i].bOn) {
			if (mapBlock[bomb[nBombIndex]->dwLocation + i].Block == NONE_BLOCK && mapBlock[bomb[nBombIndex]->dwLocation + i].Block == NONE_OBJECT) {
				for (int j = 0; j < bomb.size(); j++) {
					if (bomb[nBombIndex]->dwLocation + i == bomb[j]->dwLocation) {
						bomb[j]->bExplosion = true;
						bomb[j]->dwAni = 0;
						bomb[j]->dwAniTime = 0;
						mapBlock[bomb[j]->dwLocation].bOn = false;
						CheckBombRange(j);
						break;
					}
				}
			}
			else if (mapBlock[bomb[nBombIndex]->dwLocation + i].Block != NONE_BLOCK) {
				mapBlock[bomb[nBombIndex]->dwLocation + i].bMelting = true;
			}

			break;
		}
		nRIGHT++;
	}

	bomb[nBombIndex]->dwUp = nUP;
	bomb[nBombIndex]->dwDown = nDOWN;
	bomb[nBombIndex]->dwLeft = nLEFT;
	bomb[nBombIndex]->dwRight = nRIGHT;

	bomb[nBombIndex]->rtVCollision.left = mapBlock[bomb[nBombIndex]->dwLocation].rtBlock.left + 10;
	bomb[nBombIndex]->rtVCollision.right = bomb[nBombIndex]->rtVCollision.left + 44;
	bomb[nBombIndex]->rtVCollision.top = mapBlock[bomb[nBombIndex]->dwLocation - 13 * nUP].rtBlock.top + 10;
	bomb[nBombIndex]->rtVCollision.bottom = mapBlock[bomb[nBombIndex]->dwLocation + 13 * nDOWN].rtBlock.bottom - 10;

	bomb[nBombIndex]->rtHCollision.left = mapBlock[bomb[nBombIndex]->dwLocation - nLEFT].rtBlock.left + 10;
	bomb[nBombIndex]->rtHCollision.right = mapBlock[bomb[nBombIndex]->dwLocation + nRIGHT].rtBlock.right - 10;
	bomb[nBombIndex]->rtHCollision.top = mapBlock[bomb[nBombIndex]->dwLocation].rtBlock.top + 10;
	bomb[nBombIndex]->rtHCollision.bottom = bomb[nBombIndex]->rtHCollision.top + 44;
}

void CStory::CheckResult() {
	if (Enemy[0]->nLife <= 0 && Enemy[1]->nLife <= 0 && Enemy[2]->nLife <= 0) {
		bClear = true;

		CSoundManager::getInstance()->StopChBGM();
		CSoundManager::getInstance()->StartChBGM(STAGE_CLEAR_BGM);
	}
}

STORY_STEP CStory::Update() {
	dwCurTime = GetTickCount64();

	if (dwCurTime - dwSecondTime > 1000) { // 남은 시간 계산
		dwSecondTime = dwCurTime;
		dwSecond--;
	}

	if (dwCurTime - dwPushStartTime > 500) { // PUSH_START
		dwPushStartTime = dwCurTime;

		if (nPushStart == 61) {
			nPushStart = 76;
		}
		else {
			nPushStart = 61;
		}
	}

	if (bomber.state != P_STATE_DIE) {
		BomberControl(); //  키 입력
		CheckCollision();
	}
	else {
		bomber.SetAni(dwCurTime);
		if (dwCurTime - dwDieTime > 3000) {
			bFadeOut = true;
		}
	}

	if (bClear) {
		SetClearAni();

		if (nResultCount >= 5) {
			bFadeOut = true;
		}
	}
	else {
		CheckResult();
	}

	for (int i = 0; i < bomb.size(); i++) { // 폭탄
		if (bomb[i]->bOn) {
			bomb[i]->SetAni(dwCurTime);
			if (bomb[i]->bExplosion && bomb[i]->dwAni == 0) {
				CheckBombRange(i);
				mapBlock[bomb[i]->dwLocation].bOn = false;
			}
		}
	}

	for (int i = 0; i < item.size(); i++) { // 아이템
		if (item[i]->bOn) {
			item[i]->SetAni(dwCurTime);
		}
	}

	SetBlockAni();

	// Enemy ===========================================
	for (int i = 0; i < 3; i++) {
		if (Enemy[i]->nLife > 0) {
			Enemy[i]->SetAni(dwCurTime);

			if (Enemy[i]->rtCollision.left > 960 || Enemy[i]->rtCollision.left < 192 || Enemy[i]->rtCollision.top < 128 || Enemy[i]->rtCollision.top > 768) {
				switch (Enemy[i]->way) {
					case WAY_UP:
					{
						Enemy[i]->fYPos += 1;
						break;
					}
					case WAY_DOWN:
					{
						Enemy[i]->fYPos -= 1;
						break;
					}
					case WAY_LEFT:
					{
						Enemy[i]->fXPos += 1;
						break;
					}
					case WAY_RIGHT:
					{
						Enemy[i]->fXPos -= 1;
						break;
					}
				}

				Enemy[i]->SetRECT();
				srand((unsigned int)time(NULL));
				Enemy[i]->way = (MOVE_WAY)(rand() % 4 + 1);
			}
			else {
				bool bCheck = true;
				RECT rtCollision;
				for (int j = 0; j < 143; j++) {
					if (mapBlock[j].bOn && IntersectRect(&rtCollision, &mapBlock[j].rtBlock, &Enemy[i]->rtCollision)) {
						bCheck = false;
						switch (Enemy[i]->way) {
							case WAY_UP:
							{
								Enemy[i]->fYPos += 1;
								break;
							}
							case WAY_DOWN:
							{
								Enemy[i]->fYPos -= 1;
								break;
							}
							case WAY_LEFT:
							{
								Enemy[i]->fXPos += 1;
								break;
							}
							case WAY_RIGHT:
							{
								Enemy[i]->fXPos -= 1;
								break;
							}
						}
						break;
					}
				}

				Enemy[i]->SetRECT();

				if (!bCheck) {
					srand((unsigned int)time(NULL));
					Enemy[i]->way = (MOVE_WAY)(rand() % 4 + 1);
				}
				else {
					Enemy[i]->Move(dwCurTime);
				}
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
			storyStep = STORY_END;
		}
	}
	return storyStep;
}

void CStory::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, 0, 0, SRCCOPY);

	// 인터페이스 ======================================================================================================================
	StretchBlt(hBackbuffer, 0, 0, WIN_RIGHT, 64, hInterfaceDC, 0, 0, 304, 21, SRCCOPY); // 인터페이스 바
	
	TransparentBlt(hBackbuffer, 558, 27, 18, 18, hNumDC, (dwSecond / 60) * 6, 7, 6, 6, RGB(0, 248, 0)); // 분
	TransparentBlt(hBackbuffer, 607, 27, 18, 18, hNumDC, (dwSecond % 60) / 10 * 6, 7, 6, 6, RGB(0, 248, 0)); // 초(십)
	TransparentBlt(hBackbuffer, 632, 27, 18, 18, hNumDC, (dwSecond % 60) % 10 * 6, 7, 6, 6, RGB(0, 248, 0)); // 초(일)

	if (bomber.player == PLAYER_P1) {
		StretchBlt(hBackbuffer, 81, 0, 431, 59, hInterfaceDC, 0, 21, 113, 20, SRCCOPY); // 1P 점수창
		TransparentBlt(hBackbuffer, 190, 24, 15, 21, hNumDC, bomber.bLife * 5, 0, 5, 7, RGB(0, 248, 0)); // 1P 라이프
		int Score = nScore, nXPos = 480;
		if (nScore == 0) {
			TransparentBlt(hBackbuffer, nXPos, 24, 15, 21, hNumDC, 0, 0, 5, 7, RGB(0, 248, 0));
		}
		else {
			while (Score) {
				TransparentBlt(hBackbuffer, nXPos, 24, 15, 21, hNumDC, (Score % 10) * 5, 0, 5, 7, RGB(0, 248, 0));
				Score /= 10;
				nXPos -= 25;
			}
		}
	}
	else {
		StretchBlt(hBackbuffer, 91, 12, 396, 44, hInterfaceDC, 0, nPushStart, 106, 15, SRCCOPY); // PUSH_START
	}

	StretchBlt(hBackbuffer, 741, 12, 396, 44, hInterfaceDC, 0, nPushStart, 106, 15, SRCCOPY); // 2P PUSH_START

	// Map Tyle
	for (int i = 0; i < 143; i++) {
		if (mapBlock[i].Tyle != NONE_TYLE) {
			BitBlt(hBackbuffer, mapBlock[i].rtBlock.left, mapBlock[i].rtBlock.top, 64, 64, hTyleDC[mapBlock[i].Tyle - 1], 0, 0, SRCCOPY);
		}
	}

	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i]->bOn && bomb[i]->bExplosion) {
			TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation].rtBlock.left, mapBlock[bomb[i]->dwLocation].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni, 16, BOMB_CONETER, 16, RGB(0, 248, 0));

			for (int j = 1; j <= bomb[i]->dwUp; j++) {
				if (j == bomb[i]->dwRange) {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation - 13 * j].rtBlock.left, mapBlock[bomb[i]->dwLocation - 13 * j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_UP_ADGE, 16, 16, RGB(0, 248, 0));
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation - 13 * j].rtBlock.left, mapBlock[bomb[i]->dwLocation - 13 * j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_UP, 16, 16, RGB(0, 248, 0));
				}
			}

			for (int j = 1; j <= bomb[i]->dwDown; j++) {
				if (j == bomb[i]->dwRange) {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation + 13 * j].rtBlock.left, mapBlock[bomb[i]->dwLocation + 13 * j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_DOWN_ADGE, 16, 16, RGB(0, 248, 0));
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation + 13 * j].rtBlock.left, mapBlock[bomb[i]->dwLocation + 13 * j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_DOWN, 16, 16, RGB(0, 248, 0));
				}
			}

			for (int j = 1; j <= bomb[i]->dwLeft; j++) {
				if (j == bomb[i]->dwRange) {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation - j].rtBlock.left, mapBlock[bomb[i]->dwLocation - j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_LEFT_ADGE, 16, 16, RGB(0, 248, 0));
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation - j].rtBlock.left, mapBlock[bomb[i]->dwLocation - j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_LEFT, 16, 16, RGB(0, 248, 0));
				}
			}

			for (int j = 1; j <= bomb[i]->dwRight; j++) {
				if (j == bomb[i]->dwRange) {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation + j].rtBlock.left, mapBlock[bomb[i]->dwLocation + j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_RIGHT_ADGE, 16, 16, RGB(0, 248, 0));
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[bomb[i]->dwLocation + j].rtBlock.left, mapBlock[bomb[i]->dwLocation + j].rtBlock.top, 64, 64, hBombDC, bomb[i]->dwAni - 16 * j, BOMB_RIGHT, 16, 16, RGB(0, 248, 0));
				}

			}
		}
	}

	// Map data
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 13; j++) {
			if (mapBlock[(i * 13) + j].Object != NONE_OBJECT) {
				if (mapBlock[i * 13 + j].Object == OBJECT05) { // 링
					BitBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hObjectDC[mapBlock[(i * 13) + j].Object - 1], 0, 24, SRCCOPY);
				}
				else if (mapBlock[i * 13 + j].Object == OBJECT06) { // 돌
					TransparentBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hObjectDC[mapBlock[(i * 13) + j].Object - 1], 0, 22, 64, 64, RGB(0, 248, 0));
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top - 24, 64, 24, hObjectDC[mapBlock[(i * 13) + j].Object - 1], 0, 0, 64, 22, RGB(0, 248, 0));
					BitBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hObjectDC[mapBlock[(i * 13) + j].Object - 1], 0, 24, SRCCOPY);
				}
			}
			else if (mapBlock[(i * 13) + j].Block != NONE_BLOCK) {
				if (mapBlock[(i * 13) + j].Block == BLOCK01) {
					BitBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hBlockDC[mapBlock[(i * 13) + j].Block - 1], mapBlock[(i * 13) + j].dwAni, 0, SRCCOPY);
				}
				else if (mapBlock[(i * 13) + j].Block == BLOCK02) {
					if (mapBlock[(i * 13) + j].bMelting) {
						TransparentBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hBlockDC[mapBlock[(i * 13) + j].Block - 1], mapBlock[(i * 13) + j].dwAni, 0, 64, 64, RGB(0, 248, 0));
					}
					else {
						BitBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hBlockDC[mapBlock[(i * 13) + j].Block - 1], mapBlock[(i * 13) + j].dwAni, 0, SRCCOPY);
					}
				}
				else {
					TransparentBlt(hBackbuffer, mapBlock[(i * 13) + j].rtBlock.left, mapBlock[(i * 13) + j].rtBlock.top, 64, 64, hBlockDC[mapBlock[(i * 13) + j].Block - 1], mapBlock[(i * 13) + j].dwAni, 0, 64, 64, RGB(0, 248, 0));
				}
			}
		}
		// Enemy ======================================================================================================================
		for (int j = 0; j < 3; j++) {
			if (Enemy[j]->nLife > 0 && Enemy[j]->dwLocation / 13 == i) {
				if (Enemy[j]->kind == Matango) {
					TransparentBlt(hBackbuffer, Enemy[j]->fXPos, Enemy[j]->fYPos, 120, 120, hEnemyDC, Enemy[j]->dwAni, Enemy[j]->dwState, 32, 31, RGB(255, 0, 255));
				}
				else {
					TransparentBlt(hBackbuffer, Enemy[j]->fXPos, Enemy[j]->fYPos, 120, 120, hEnemyDC, Enemy[j]->dwAni, Enemy[j]->kind, 32, 31, RGB(255, 0, 255));
				}
			}
		}

		for (int j = 0; j < bomb.size(); j++) { // 폭탄
			if (bomb[j]->bOn && bomb[j]->dwLocation / 13 == i && !bomb[j]->bExplosion) {
				TransparentBlt(hBackbuffer, mapBlock[bomb[j]->dwLocation].rtBlock.left, mapBlock[bomb[j]->dwLocation].rtBlock.top, 64, 64, hBombDC, bomb[j]->dwAni, 0, 16, 16, RGB(0, 248, 0));
			}
		}

		for (int j = 0; j < item.size(); j++) { // 아이템
			if (item[j]->bOn && item[j]->dwLocation / 13 == i) {
				TransparentBlt(hBackbuffer, mapBlock[item[j]->dwLocation].rtBlock.left, mapBlock[item[j]->dwLocation].rtBlock.top - 28, 64, 92, hItemDC, item[j]->dwAni, item[j]->kind, 64, 92, RGB(0, 248, 0));
			}
		}

		if (bomber.state != P_STATE_DIE) {
			if (mapBlock[i * 13].rtBlock.top <= bomber.rtCollision.top && mapBlock[i * 13].rtBlock.bottom >= bomber.rtCollision.top) {
				TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos + 60, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
				TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state, 32, 32, RGB(255, 0, 255)); //대가리
			}
		}
	}

	if (bomber.state == P_STATE_DIE) {
		if (bomber.dwAni < 128) {
			TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos + 60, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
			TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state, 32, 32, RGB(255, 0, 255)); //대가리
		}
		else {
			TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos + bomber.dwDieCount * 4, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state, 32, 32, RGB(255, 0, 255)); //대가리
			TransparentBlt(hBackbuffer, bomber.fXPos, bomber.fYPos + 60 - bomber.dwDieCount * 5, 120, 120, h1PDC, bomber.direction + bomber.dwAni, bomber.state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
		}
	}

	if (bClear) {
		TransparentBlt(hBackbuffer, 250, 200, 702, 330, hClearDC, dwClearAni, 0, 702, 330, RGB(0, 248, 0));
	}

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}