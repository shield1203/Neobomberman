#include"CMapSelect.h"
#include"CSoundManager.h"

CMapSelect::CMapSelect(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	hBackgroundDC = CreateBmpDC("Resource\\MapSelect\\Background.bmp");
	hSelectorDC = CreateBmpDC("Resource\\MapSelect\\Selector.bmp");
	hNumDC = CreateBmpDC("Resource\\MapSelect\\Num.bmp");

	Init();
}

CMapSelect::~CMapSelect() {
	DeleteDC(hBackgroundDC);
	DeleteDC(hSelectorDC);
	DeleteDC(hNumDC);
	
	for (int i = 0; i < miniMap.size(); i++) {
		DeleteDC(miniMap[i]->hMiniMapDC);
		MiniMap* mini = miniMap[i];
		delete mini;
	}
	miniMap.clear();
}

void CMapSelect::Init() {
	dwCurTime = 0;
	dwAni = 0;
	dwAniTime = 0;
	dwButtonTime = 0;
	nSelect = 0;

	LoadMiniMap(); // 맵 데이터 로드

	nAlpha = 255;
	bFadeIn = true;
	bFadeOut = false;
	BlendFunction.SourceConstantAlpha = nAlpha;
}

void CMapSelect::LoadMiniMap() {
	// xml파일 이름 MapList.xml
	TiXmlDocument doc;
	doc.LoadFile("MapList.xml");

	TiXmlElement* pRoot = doc.FirstChildElement("MAPS");
	if (!pRoot) return;

	TiXmlElement* pElem = pRoot->FirstChildElement("NAME");

	while (pElem) {
		MiniMap* map = new MiniMap;

		map->strFileName = pElem->GetText();
		map->strFileName.append(".xml");

		string filename = "Resource\\MapSelect\\";
		filename.append(pElem->GetText());
		filename.append(".bmp");
		map->hMiniMapDC = CreateBmpDC(filename.c_str());

		miniMap.push_back(map);

		pElem = pElem->NextSiblingElement();
	}
}

void CMapSelect::CheckKey() {
	if ((GetKeyState(VK_RIGHT) < 0) && dwCurTime - dwButtonTime > 150) {
		dwButtonTime = dwCurTime;
		
		nSelect++;

		nSelect %= miniMap.size();

		CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
	}
	else if ((GetKeyState(VK_LEFT) < 0) && dwCurTime - dwButtonTime > 150) {
		dwButtonTime = dwCurTime;
		
		nSelect--;

		if (nSelect < 0) {
			nSelect = (miniMap.size() - 1);
		}
		else {
			nSelect %= miniMap.size();
		}

		CSoundManager::getInstance()->StartChEffect(SOUND_BUTTON);
	}
	else if ((GetKeyState(KEY_K) < 0) && dwCurTime - dwButtonTime > 150) {
		dwButtonTime = dwCurTime;

		strMapFileName = miniMap[nSelect]->strFileName;
		bFadeOut = true;

		CSoundManager::getInstance()->StartChEffect(SOUND_SELECT);
	}
}

void CMapSelect::SetAni() {
	if (dwCurTime - dwAniTime > 150) {
		dwAniTime = dwCurTime;

		if (dwAni >= MAP_SELECTOR_MAX) {
			dwAni = 0;
		}
		else {
			dwAni += MAP_SELECTOR_WIDTH;
		}
	}
}

void CMapSelect::Update() {
	dwCurTime = GetTickCount64();

	if (!bFadeIn && !bFadeOut) {
		CheckKey();
	}

	if (bFadeIn && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeIn();
	}

	if (bFadeOut && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeOut();

		if (!bFadeOut) {
			CBattleManager::getInstance(hWnd, hBackbuffer)->battleStep = BATTLE_BOMBER_SELECT;
		}
	}

	SetAni();
}

void CMapSelect::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC, 0, 0, SRCCOPY);

	if (nSelect - 1 < 0) {
		BitBlt(hBackbuffer, -59, 417, 446, 446, miniMap[miniMap.size()-1]->hMiniMapDC, 0, 0, SRCCOPY);
	}
	else {
		BitBlt(hBackbuffer, -59, 417, 446, 446, miniMap[nSelect-1]->hMiniMapDC, 0, 0, SRCCOPY);
	}
	BitBlt(hBackbuffer, 387, 417, 446, 446, miniMap[nSelect]->hMiniMapDC, 0, 0, SRCCOPY);
	BitBlt(hBackbuffer, 833, 417, 446, 446, miniMap[(nSelect + 1) % miniMap.size()]->hMiniMapDC, 0, 0, SRCCOPY);

	TransparentBlt(hBackbuffer, 321, 287, MAP_SELECTOR_WIDTH, MAP_SELECTOR_HEIGHT, hSelectorDC, dwAni, 0, MAP_SELECTOR_WIDTH, MAP_SELECTOR_HEIGHT, RGB(0, 248, 0));

	BitBlt(hBackbuffer, 575, 321, 62, 62, hNumDC, nSelect * 62, 0, SRCCOPY);

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}