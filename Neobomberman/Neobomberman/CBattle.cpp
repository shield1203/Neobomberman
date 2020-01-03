#include"CBattle.h"
#include"CMapSelect.h"
#include"CBomberSelect.h"
#include"CSoundManager.h"

CBattle::CBattle(HWND hWnd, HDC hBackbuffer) : CGameFrame(hWnd, hBackbuffer) {
	hInterfaceDC = CreateBmpDC("Resource\\Battle\\Interface.bmp");
	hNumDC = CreateBmpDC("Resource\\Battle\\Num.bmp");
	hItemDC = CreateBmpDC("Resource\\Battle\\Item.bmp");
	hBombDC = CreateBmpDC("Resource\\Battle\\Bomb.bmp");

	//BomberDC
	hBomberDC[0] = CreateBmpDC("Resource\\Battle\\White.bmp");
	hBomberDC[1] = CreateBmpDC("Resource\\Battle\\Black.bmp");
	hBomberDC[2] = CreateBmpDC("Resource\\Battle\\Red.bmp");
	hBomberDC[3] = CreateBmpDC("Resource\\Battle\\Blue.bmp");
	hBomberDC[4] = CreateBmpDC("Resource\\Battle\\Rubber.bmp");
	hBomberDC[5] = CreateBmpDC("Resource\\Battle\\Fake.bmp");
	hBomberDC[6] = CreateBmpDC("Resource\\Battle\\Cat.bmp");
	hBomberDC[7] = CreateBmpDC("Resource\\Battle\\Honey.bmp");
	hBomberDC[8] = CreateBmpDC("Resource\\Battle\\Hayate.bmp");
	hBomberDC[9] = CreateBmpDC("Resource\\Battle\\Kotetsu.bmp");
	hBomberDC[10] = CreateBmpDC("Resource\\Battle\\Gold.bmp");
	hBomberDC[11] = CreateBmpDC("Resource\\Battle\\Atomic.bmp");

	MapDCLoad();
	bOn = false;
}

CBattle::~CBattle() {
	DeleteDC(hInterfaceDC);
	DeleteDC(hNumDC);
	DeleteDC(hItemDC);
	DeleteDC(hBombDC);

	for (int i = 0; i < 12; i++) {
		DeleteDC(hBomberDC[i]);
	}

	// MapDC
	for (int i = 0; i < 4; i++) {
		DeleteDC(hBackgroundDC[i]);
	}

	for (int i = 0; i < 22; i++) {
		DeleteDC(hTyleDC[i]);
	}

	for (int i = 0; i < 7; i++) {
		DeleteDC(hObjectDC[i]);
	}

	for (int i = 0; i < 3; i++) {
		DeleteDC(hBlockDC[i]);
	}

	//Bomb, Item
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

void CBattle::Init() {
	bOn = true;

	battleStep = BATTLE_PLAY;
	dwCurTime = 0;

	string strMapFileName = CBattleManager::getInstance(hWnd, hBackbuffer)->MapSelect->strMapFileName;
 	LoadMapData(strMapFileName);

	float fSpeed;
	if (strMapFileName == "Village.xml" || strMapFileName == "Ling.xml") {
		fSpeed = 2;

		CSoundManager::getInstance()->StopChBGM();
		CSoundManager::getInstance()->StartChBGM(BATTLE_MAP1_BGM);
	}
	else {
		fSpeed = 4;

		CSoundManager::getInstance()->StopChBGM();
		CSoundManager::getInstance()->StartChBGM(BATTLE_MAP2_BGM);
	}

	bomber[0].InitBomber(CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[0].player,
		CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[0].bomber, fSpeed);
	bomber[1].InitBomber(CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[1].player,
		CBattleManager::getInstance(hWnd, hBackbuffer)->BomberSelect->Player[1].bomber, fSpeed);

	dwSecond = 180;
	dwSecondTime = 0;
	dwDieTime = 0;

	for (int i = 0; i < item.size(); i++) {
		item[i]->bOn = false;
	}

	for (int i = 0; i < bomb.size(); i++) {
		bomb[i]->bOn = false;
	}

	nAlpha = 255;
	bFadeIn = true;
	bFadeOut = false;
	BlendFunction.SourceConstantAlpha = nAlpha;

	CSoundManager::getInstance()->StartChAnnouncer(SOUND_READY);
}

void CBattle::LoadMapData(string strMapFilrName) {
	// Map Data 
	TiXmlDocument doc;
	doc.LoadFile(strMapFilrName.c_str());

	TiXmlElement* pRoot = doc.FirstChildElement("Map");
	if (!pRoot) return;

	TiXmlElement* pElem = pRoot->FirstChildElement("Background");
	string strFileName = pElem->GetText();
	dwBackground = atoi(strFileName.c_str());

	pElem = pRoot->FirstChildElement("MapBlock");
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

void CBattle::MapDCLoad() {
	for (int i = 0; i < 4; i++) {
		string filename = "Resource\\Battle\\Background0.bmp";
		filename.insert(27, to_string(i+1));
		hBackgroundDC[i] = CreateBmpDC(filename.c_str());
	}

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

void CBattle::BomberControl() {
	if (bomber[0].player == PLAYER_P1AI || bomber[1].player == PLAYER_P2AI) {
		if (GetAsyncKeyState(KEY_1) < 0) {

		}

		if (GetAsyncKeyState(KEY_2) < 0) {

		}
	}

	if (bomber[0].player == PLAYER_P1) {
		if (GetAsyncKeyState(KEY_K) < 0 && dwCurTime - bomber[0].dwButtonTime > 190) { // 폭탄 설치
			bomber[0].state = P_STATE_MOVE;
			bomber[0].dwButtonTime = dwCurTime;

			bool bCheck = true;
			if (mapBlock[bomber[0].dwLocation].bOn) {
				bCheck = false;
			}

			int nBombCount = 0;
			for (int i = 0; i < bomb.size(); i++) {
				if (bomb[i]->player == PLAYER_P1) {
					nBombCount++;
				}
			}

			if (bCheck && bomber[0].dwMaxBombCount > nBombCount) {
				bool bCheck = false;
				for (int i = 0; i < bomb.size(); i++) {
					if (!bomb[i]->bOn) {
						bomb[i]->SetBomb(bomber[0].player, bomber[0].dwLocation, bomber[0].dwBombRange);
						bCheck = true;
						break;
					}
				}

				if (!bCheck) {
					CBomb *addBomb = new CBomb();
					addBomb->SetBomb(bomber[0].player, bomber[0].dwLocation, bomber[0].dwBombRange);
					bomb.push_back(addBomb);
				}
				mapBlock[bomber[0].dwLocation].bOn = true;

				CSoundManager::getInstance()->StartSubChEffect(SOUND_SET_BOMB);
			}
		}
		else if (GetAsyncKeyState(KEY_L) < 0 && bomber[0].bomber == GOLD) {
			bomber[0].state = P_STATE_DASH;
			bomber[0].SetAni(dwCurTime);
			if (GetAsyncKeyState(VK_UP) < 0) {
				bomber[0].direction = PLAYER_UP;
				if (bomber[0].fYPos - bomber[0].fSpeed * 2 > 74) {
					if (bomber[0].dwLocation < 13) {
						bomber[0].fYPos -= bomber[0].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[0].dwLocation - 13].bOn) {
							if (mapBlock[bomber[0].dwLocation - 13].rtBlock.left + 32 == bomber[0].pLocation.x) {
								bomber[0].fYPos -= bomber[0].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[0].dwLocation].rtBlock.bottom - 32 < bomber[0].pLocation.y) {
								bomber[0].fYPos -= bomber[0].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(VK_DOWN) < 0) {
				bomber[0].direction = PLAYER_DOWN;
				bomber[0].SetAni(dwCurTime);
				if (bomber[0].fYPos + bomber[0].fSpeed * 2 < 716) {
					if (bomber[0].dwLocation > 129) {
						bomber[0].fYPos += bomber[0].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[0].dwLocation + 13].bOn) {
							if (mapBlock[bomber[0].dwLocation + 13].rtBlock.left + 32 == bomber[0].pLocation.x) {
								bomber[0].fYPos += bomber[0].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[0].dwLocation].rtBlock.bottom - 32 > bomber[0].pLocation.y) {
								bomber[0].fYPos += bomber[0].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(VK_LEFT) < 0) {
				bomber[0].direction = PLAYER_LEFT;
				bomber[0].SetAni(dwCurTime);
				if (bomber[0].fXPos - bomber[0].fSpeed * 2 > 163) {
					if (bomber[0].dwLocation % 13 == 0) {
						bomber[0].fXPos -= bomber[0].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[0].dwLocation - 1].bOn) {
							if (mapBlock[bomber[0].dwLocation - 1].rtBlock.top + 32 == bomber[0].pLocation.y) {
								bomber[0].fXPos -= bomber[0].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[0].dwLocation].rtBlock.left + 32 < bomber[0].pLocation.x) {
								bomber[0].fXPos -= bomber[0].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(VK_RIGHT) < 0) {
				bomber[0].direction = PLAYER_RIGHT;
				bomber[0].SetAni(dwCurTime);
				if (bomber[0].fXPos + bomber[0].fSpeed * 2 < 933) {
					if (bomber[0].dwLocation % 13 == 12) {
						bomber[0].fXPos += bomber[0].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[0].dwLocation + 1].bOn) {
							if (mapBlock[bomber[0].dwLocation + 1].rtBlock.top + 32 == bomber[0].pLocation.y) {
								bomber[0].fXPos += bomber[0].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[0].dwLocation].rtBlock.left + 32 > bomber[0].pLocation.x) {
								bomber[0].fXPos += bomber[0].fSpeed * 2;
							}
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_UP) < 0) {
			bomber[0].state = P_STATE_MOVE;
			bomber[0].direction = PLAYER_UP;
			bomber[0].SetAni(dwCurTime);
			
			if (bomber[0].fYPos - bomber[0].fSpeed > 74) {
				if (bomber[0].dwLocation < 13) {
					bomber[0].fYPos -= bomber[0].fSpeed;
				}
				else {
					if (!mapBlock[bomber[0].dwLocation - 13].bOn) {
						if (mapBlock[bomber[0].dwLocation - 13].rtBlock.left + 32 == bomber[0].pLocation.x) {
							bomber[0].fYPos -= bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation - 13].rtBlock.left + 32 > bomber[0].pLocation.x) {
							bomber[0].fXPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation - 13].rtBlock.left + 32 < bomber[0].pLocation.x) {
							bomber[0].fXPos -= bomber[0].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[0].dwLocation].rtBlock.bottom -32 < bomber[0].pLocation.y){
							bomber[0].fYPos -= bomber[0].fSpeed;
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0) {
			bomber[0].state = P_STATE_MOVE;
			bomber[0].direction = PLAYER_DOWN;
			bomber[0].SetAni(dwCurTime);
			
			if (bomber[0].fYPos + bomber[0].fSpeed < 716) {
				if (bomber[0].dwLocation > 129) {
					bomber[0].fYPos += bomber[0].fSpeed;
				}
				else {
					if (!mapBlock[bomber[0].dwLocation + 13].bOn) {
						if (mapBlock[bomber[0].dwLocation + 13].rtBlock.left + 32 == bomber[0].pLocation.x) {
							bomber[0].fYPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation + 13].rtBlock.left + 32 > bomber[0].pLocation.x) {
							bomber[0].fXPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation + 13].rtBlock.left + 32 < bomber[0].pLocation.x) {
							bomber[0].fXPos -= bomber[0].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[0].dwLocation].rtBlock.bottom - 32 > bomber[0].pLocation.y) {
							bomber[0].fYPos += bomber[0].fSpeed;
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_LEFT) < 0) {
			bomber[0].state = P_STATE_MOVE;
			bomber[0].direction = PLAYER_LEFT;
			bomber[0].SetAni(dwCurTime);

			if (bomber[0].fXPos - bomber[0].fSpeed > 163) {
				if (bomber[0].dwLocation % 13 == 0) {
					bomber[0].fXPos -= bomber[0].fSpeed;
				}
				else {
					if (!mapBlock[bomber[0].dwLocation - 1].bOn) {
						if (mapBlock[bomber[0].dwLocation - 1].rtBlock.top + 32 == bomber[0].pLocation.y) {
							bomber[0].fXPos -= bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation - 1].rtBlock.top + 32 > bomber[0].pLocation.y) {
							bomber[0].fYPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation - 1].rtBlock.top + 32 < bomber[0].pLocation.y) {
							bomber[0].fYPos -= bomber[0].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[0].dwLocation].rtBlock.left + 32 < bomber[0].pLocation.x) {
							bomber[0].fXPos -= bomber[0].fSpeed;
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) < 0) {
			bomber[0].state = P_STATE_MOVE;
			bomber[0].direction = PLAYER_RIGHT;
			bomber[0].SetAni(dwCurTime);

			if (bomber[0].fXPos + bomber[0].fSpeed < 933) {
				if (bomber[0].dwLocation % 13 == 12) {
					bomber[0].fXPos += bomber[0].fSpeed;
				}
				else {
					if (!mapBlock[bomber[0].dwLocation + 1].bOn) {
						if (mapBlock[bomber[0].dwLocation + 1].rtBlock.top + 32 == bomber[0].pLocation.y) {
							bomber[0].fXPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation + 1].rtBlock.top + 32 > bomber[0].pLocation.y) {
							bomber[0].fYPos += bomber[0].fSpeed;
						}
						else if (mapBlock[bomber[0].dwLocation + 1].rtBlock.top + 32 < bomber[0].pLocation.y) {
							bomber[0].fYPos -= bomber[0].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[0].dwLocation].rtBlock.left + 32 > bomber[0].pLocation.x) {
							bomber[0].fXPos += bomber[0].fSpeed;
						}
					}
				}
			}
		}
		else {
			bomber[0].dwAni = 0;
		}
	}

	if (bomber[1].player == PLAYER_P2) {
		if (GetAsyncKeyState(KEY_A) < 0 && dwCurTime - bomber[1].dwButtonTime > 190) { // 폭탄 설치
			bomber[1].dwButtonTime = dwCurTime;
			
			bool bCheck = true;
			if (mapBlock[bomber[1].dwLocation].bOn) {
				bCheck = false;
			}

			int nBombCount = 0;
			for (int i = 0; i < bomb.size(); i++) {
				if (bomb[i]->player == PLAYER_P2) {
					nBombCount++;
				}
			}

			if (bCheck && bomber[1].dwMaxBombCount > nBombCount) {
				bool bCheck = false;
				for (int i = 0; i < bomb.size(); i++) {
					if (!bomb[i]->bOn) {
						bomb[i]->SetBomb(bomber[1].player, bomber[1].dwLocation, bomber[1].dwBombRange);
						bCheck = true;
						break;
					}
				}

				if (!bCheck) {
					CBomb* addBomb = new CBomb();
					addBomb->SetBomb(bomber[1].player, bomber[1].dwLocation, bomber[1].dwBombRange);
					bomb.push_back(addBomb);
				}
				mapBlock[bomber[1].dwLocation].bOn = true;

				CSoundManager::getInstance()->StartSubChEffect(SOUND_SET_BOMB);
			}
		}
		else if (GetAsyncKeyState(KEY_S) < 0 && bomber[1].bomber == GOLD) {
			bomber[1].state = P_STATE_DASH;
			bomber[1].SetAni(dwCurTime);
			if (GetAsyncKeyState(KEY_R) < 0) {
				bomber[1].direction = PLAYER_UP;
				if (bomber[1].fYPos - bomber[1].fSpeed * 2 > 74) {
					if (bomber[1].dwLocation < 13) {
						bomber[1].fYPos -= bomber[1].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[1].dwLocation - 13].bOn) {
							if (mapBlock[bomber[1].dwLocation - 13].rtBlock.left + 32 == bomber[1].pLocation.x) {
								bomber[1].fYPos -= bomber[1].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[1].dwLocation].rtBlock.bottom - 32 < bomber[1].pLocation.y) {
								bomber[1].fYPos -= bomber[1].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(KEY_F) < 0) {
				bomber[1].direction = PLAYER_DOWN;
				bomber[1].SetAni(dwCurTime);
				if (bomber[1].fYPos + bomber[1].fSpeed * 2 < 716) {
					if (bomber[1].dwLocation > 129) {
						bomber[1].fYPos += bomber[1].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[1].dwLocation + 13].bOn) {
							if (mapBlock[bomber[1].dwLocation + 13].rtBlock.left + 32 == bomber[1].pLocation.x) {
								bomber[1].fYPos += bomber[1].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[1].dwLocation].rtBlock.bottom - 32 > bomber[1].pLocation.y) {
								bomber[1].fYPos += bomber[1].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(KEY_D) < 0) {
				bomber[1].direction = PLAYER_LEFT;
				bomber[1].SetAni(dwCurTime);
				if (bomber[1].fXPos - bomber[1].fSpeed * 2 > 163) {
					if (bomber[1].dwLocation % 13 == 0) {
						bomber[1].fXPos -= bomber[1].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[1].dwLocation - 1].bOn) {
							if (mapBlock[bomber[1].dwLocation - 1].rtBlock.top + 32 == bomber[1].pLocation.y) {
								bomber[1].fXPos -= bomber[1].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[1].dwLocation].rtBlock.left + 32 < bomber[1].pLocation.x) {
								bomber[1].fXPos -= bomber[1].fSpeed * 2;
							}
						}
					}
				}
			}
			else if (GetAsyncKeyState(KEY_G) < 0) {
				bomber[1].direction = PLAYER_RIGHT;
				bomber[1].SetAni(dwCurTime);
				if (bomber[1].fXPos + bomber[1].fSpeed * 2 < 933) {
					if (bomber[1].dwLocation % 13 == 12) {
						bomber[1].fXPos += bomber[1].fSpeed * 2;
					}
					else {
						if (!mapBlock[bomber[1].dwLocation + 1].bOn) {
							if (mapBlock[bomber[1].dwLocation + 1].rtBlock.top + 32 == bomber[1].pLocation.y) {
								bomber[1].fXPos += bomber[1].fSpeed * 2;
							}
						}
						else {
							if (mapBlock[bomber[1].dwLocation].rtBlock.left + 32 > bomber[1].pLocation.x) {
								bomber[1].fXPos += bomber[1].fSpeed * 2;
							}
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(KEY_R) < 0) {
			bomber[1].state = P_STATE_MOVE;
			bomber[1].direction = PLAYER_UP;
			bomber[1].SetAni(dwCurTime);

			if (bomber[1].fYPos - bomber[1].fSpeed > 74) {
				if (bomber[1].dwLocation < 13) {
					bomber[1].fYPos -= bomber[1].fSpeed;
				}
				else {
					if (!mapBlock[bomber[1].dwLocation - 13].bOn) {
						if (mapBlock[bomber[1].dwLocation - 13].rtBlock.left + 32 == bomber[1].pLocation.x) {
							bomber[1].fYPos -= bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation - 13].rtBlock.left + 32 > bomber[1].pLocation.x) {
							bomber[1].fXPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation - 13].rtBlock.left + 32 < bomber[1].pLocation.x) {
							bomber[1].fXPos -= bomber[1].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[1].dwLocation].rtBlock.bottom - 32 < bomber[1].pLocation.y) {
							bomber[1].fYPos -= bomber[1].fSpeed;
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(KEY_F) < 0) {
			bomber[1].state = P_STATE_MOVE;
			bomber[1].direction = PLAYER_DOWN;
			bomber[1].SetAni(dwCurTime);

			if (bomber[1].fYPos + bomber[1].fSpeed < 716) {
				if (bomber[1].dwLocation > 129) {
					bomber[1].fYPos += bomber[1].fSpeed;
				}
				else {
					if (!mapBlock[bomber[1].dwLocation + 13].bOn) {
						if (mapBlock[bomber[1].dwLocation + 13].rtBlock.left + 32 == bomber[1].pLocation.x) {
							bomber[1].fYPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation + 13].rtBlock.left + 32 > bomber[1].pLocation.x) {
							bomber[1].fXPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation + 13].rtBlock.left + 32 < bomber[1].pLocation.x) {
							bomber[1].fXPos -= bomber[1].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[1].dwLocation].rtBlock.bottom - 32 > bomber[1].pLocation.y) {
							bomber[1].fYPos += bomber[1].fSpeed;
						}
					}
				}

			}
		}
		else if (GetAsyncKeyState(KEY_D) < 0) {
			bomber[1].state = P_STATE_MOVE;
			bomber[1].direction = PLAYER_LEFT;
			bomber[1].SetAni(dwCurTime);

			if (bomber[1].fXPos - bomber[1].fSpeed > 163) {
				if (bomber[1].dwLocation % 13 == 0) {
					bomber[1].fXPos -= bomber[1].fSpeed;
				}
				else {
					if (!mapBlock[bomber[1].dwLocation - 1].bOn) {
						if (mapBlock[bomber[1].dwLocation - 1].rtBlock.top + 32 == bomber[1].pLocation.y) {
							bomber[1].fXPos -= bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation - 1].rtBlock.top + 32 > bomber[1].pLocation.y) {
							bomber[1].fYPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation - 1].rtBlock.top + 32 < bomber[1].pLocation.y) {
							bomber[1].fYPos -= bomber[1].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[1].dwLocation].rtBlock.left + 32 < bomber[1].pLocation.x) {
							bomber[1].fXPos -= bomber[1].fSpeed;
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(KEY_G) < 0) {
			bomber[1].state = P_STATE_MOVE;
			bomber[1].direction = PLAYER_RIGHT;
			bomber[1].SetAni(dwCurTime);

			if (bomber[1].fXPos + bomber[1].fSpeed < 933) {
				if (bomber[1].dwLocation % 13 == 12) {
					bomber[1].fXPos += bomber[1].fSpeed;
				}
				else {
					if (!mapBlock[bomber[1].dwLocation + 1].bOn) {
						if (mapBlock[bomber[1].dwLocation + 1].rtBlock.top + 32 == bomber[1].pLocation.y) {
							bomber[1].fXPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation + 1].rtBlock.top + 32 > bomber[1].pLocation.y) {
							bomber[1].fYPos += bomber[1].fSpeed;
						}
						else if (mapBlock[bomber[1].dwLocation + 1].rtBlock.top + 32 < bomber[1].pLocation.y) {
							bomber[1].fYPos -= bomber[1].fSpeed;
						}
					}
					else {
						if (mapBlock[bomber[1].dwLocation].rtBlock.left + 32 > bomber[1].pLocation.x) {
							bomber[1].fXPos += bomber[1].fSpeed;
						}
					}
				}
			}
		}
		else {
			bomber[1].dwAni = 0;
		}
	}

	bomber[0].SetRECT();
	bomber[1].SetRECT();
	LocationCheck();
}

void CBattle::LocationCheck() {
	for (int i = 0; i < 143; i++) {
		if (bomber[0].pLocation.x >= mapBlock[i].rtBlock.left && bomber[0].pLocation.x <= mapBlock[i].rtBlock.right
			&& bomber[0].pLocation.y >= mapBlock[i].rtBlock.top && bomber[0].pLocation.y <= mapBlock[i].rtBlock.bottom) {
			bomber[0].dwLocation = i;
		}

		if (bomber[1].pLocation.x >= mapBlock[i].rtBlock.left && bomber[1].pLocation.x <= mapBlock[i].rtBlock.right
			&& bomber[1].pLocation.y >= mapBlock[i].rtBlock.top && bomber[1].pLocation.y <= mapBlock[i].rtBlock.bottom) {
			bomber[1].dwLocation = i;
		}
	}
}

void CBattle::CheckBombRange(int nBombIndex) {
	int nUP = 0;
	for (int i = 1; i <= bomb[nBombIndex]->dwRange; i++) { // UP
		if ((int)bomb[nBombIndex]->dwLocation - (13 * i) < 0 || bomb[nBombIndex]->dwLocation < 13) {
			break;
		}
		else if(mapBlock[bomb[nBombIndex]->dwLocation - 13 * i].bOn){
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

void CBattle::CheckCollision() {
	RECT rtCollision;
	for (int i = 0; i < bomb.size(); i++) {
		if (bomb[i]->bExplosion) {
			if (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &bomber[0].rtCollision) ||
				IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &bomber[0].rtCollision)) {
				bomber[0].state = P_STATE_DIE;
				bomber[0].direction = PLAYER_DIE;
				bomber[0].dwAni = 0;

				dwDieTime = GetTickCount64();

				CSoundManager::getInstance()->StartChAnnouncer(SOUND_WIN);

				break;
			}

			if (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &bomber[1].rtCollision) ||
				IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &bomber[1].rtCollision)) {
				bomber[1].state = P_STATE_DIE;
				bomber[1].direction = PLAYER_DIE;
				bomber[1].dwAni = 0;
				
				dwDieTime = GetTickCount64();

				CSoundManager::getInstance()->StartChAnnouncer(SOUND_WIN);

				break;
			}

			for (int j = 0; j < item.size(); j++) {
				if (IntersectRect(&rtCollision, &bomb[i]->rtHCollision, &item[j]->rtCollision) ||
					IntersectRect(&rtCollision, &bomb[i]->rtVCollision, &item[j]->rtCollision)) {
					item[j]->bOn = false;
				}
			}
		}
	}

	for (int i = 0; i < item.size(); i++) {
		if (item[i]->bOn) {
			if (IntersectRect(&rtCollision, &item[i]->rtCollision, &bomber[0].rtCollision)) {
				item[i]->bOn = false;
				switch (item[i]->kind)
				{
					case FIRE: {
						bomber[0].dwBombRange += 1;
						break;
					}
					case BOMB: {
						bomber[0].dwMaxBombCount += 1;
						break;
					}
				}
				CSoundManager::getInstance()->StartChEffect(SOUND_ITEM);
			}

			if (IntersectRect(&rtCollision, &item[i]->rtCollision, &bomber[1].rtCollision)) {
				item[i]->bOn = false;
				switch (item[i]->kind)
				{
					case FIRE: {
						bomber[1].dwBombRange += 1;
						break;
					}
					case BOMB: {
						bomber[1].dwMaxBombCount += 1;
						break;
					}
				}
				CSoundManager::getInstance()->StartChEffect(SOUND_ITEM);
			}
		}
	}
}

void CBattle::SetBlockAni() {
	for (int i = 0; i < 143; i++) {
		if (mapBlock[i].bMelting) {
			switch (mapBlock[i].Block) {
				case BLOCK01: {
					if (dwCurTime - mapBlock[i].dwAniTime > 85) { // 680
						mapBlock[i].dwAniTime = dwCurTime;

						mapBlock[i].dwAni += 64;

						if (mapBlock[i].dwAni ==512) {
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

BATTLE_STEP CBattle::Update() {
	dwCurTime = GetTickCount64();

	if (dwCurTime - dwSecondTime > 1000) { // 남은 시간 계산
		dwSecondTime = dwCurTime;
		dwSecond--;
	}

	if (bomber[0].state != P_STATE_DIE && bomber[1].state != P_STATE_DIE) {
		BomberControl(); //  키 입력
		CheckCollision();
	}
	else if(bomber[0].state == P_STATE_DIE){
		bomber[0].SetAni(dwCurTime);
		if (dwCurTime - dwDieTime > 3000) {
			bFadeOut = true;
		}
	}
	else if (bomber[1].state == P_STATE_DIE) {
		bomber[1].SetAni(dwCurTime);
		if (dwCurTime - dwDieTime > 3000) {
			bFadeOut = true;
		}
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

	SetBlockAni(); // 블록 녹는지 체크

	if (bFadeIn && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeIn();
	}

	if (bFadeOut && dwCurTime - dwAlphaTime > 10) {
		dwAlphaTime = dwCurTime;
		FadeOut();

		if (!bFadeOut) {
			bOn = false;
			battleStep = BATTLE_RESULT;
		}
	}

	return battleStep;
}

void CBattle::Render() {
	BitBlt(hBackbuffer, 0, 0, WIN_RIGHT, WIN_BOTTOM, hBackgroundDC[dwBackground], 0, 0, SRCCOPY); // 배경

	for (int i = 0;i < 143;i++) {
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

	for (int i = 0; i < 11; i++) {
		for (int j = 0;j < 13;j++) {
			if (mapBlock[(i * 13) + j].Object != NONE_OBJECT) {
				if (mapBlock[i*13 + j].Object == OBJECT05) { // 링
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
		
		// 봄버맨 1P
		if (bomber[0].state != P_STATE_DIE) {
			if (mapBlock[i * 13].rtBlock.top <= bomber[0].rtCollision.top && mapBlock[i * 13].rtBlock.bottom >= bomber[0].rtCollision.top) {
				TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos + 60, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
				TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state, 32, 32, RGB(255, 0, 255)); //대가리
			}
		}

		// 봄버맨 2P
		if (bomber[1].state != P_STATE_DIE) {
			if (mapBlock[i * 13].rtBlock.top <= bomber[1].rtCollision.top && mapBlock[i * 13].rtBlock.bottom >= bomber[1].rtCollision.top) {
				TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos + 60, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
				TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state, 32, 32, RGB(255, 0, 255)); //대가리
			}
		}
	}

	if (bomber[0].state == P_STATE_DIE) {
		if (bomber[0].dwAni < 128) {
			TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos + 60, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
			TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state, 32, 32, RGB(255, 0, 255)); //대가리
		}
		else {
			TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos + bomber[0].dwDieCount * 4, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state, 32, 32, RGB(255, 0, 255)); //대가리
			TransparentBlt(hBackbuffer, bomber[0].fXPos, bomber[0].fYPos + 60 - bomber[0].dwDieCount * 5, 120, 120, hBomberDC[bomber[0].bomber], bomber[0].direction + bomber[0].dwAni, bomber[0].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
		}
	}

	if (bomber[1].state == P_STATE_DIE) {
		if (bomber[1].dwAni < 128) {
			TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos + 60, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
			TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state, 32, 32, RGB(255, 0, 255)); //대가리
		}
		else {
			TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos + bomber[1].dwDieCount * 4, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state, 32, 32, RGB(255, 0, 255)); //대가리
			TransparentBlt(hBackbuffer, bomber[1].fXPos, bomber[1].fYPos + 60 - bomber[1].dwDieCount * 5, 120, 120, hBomberDC[bomber[1].bomber], bomber[1].direction + bomber[1].dwAni, bomber[1].state + 32, 32, 32, RGB(255, 0, 255)); // 몸통
		}
	}

	//FrameRect(hBackbuffer, &bomber[0].rtCollision, 0); // 1P RECT

	// 인터페이스 
	StretchBlt(hBackbuffer, 0, 0, WIN_RIGHT, 64, hInterfaceDC, 0, 0, 304, 21, SRCCOPY); // 인터페이스 바
	StretchBlt(hBackbuffer, 162, 12, 50, 45, hInterfaceDC, 0, 21 + (bomber[0].bomber * 14), 16, 14, SRCCOPY); // 1P 초상화
	StretchBlt(hBackbuffer, 390, 12, 50, 45, hInterfaceDC, 0, 21 + (bomber[1].bomber * 14), 16, 14, SRCCOPY);

	TransparentBlt(hBackbuffer, 556, 27, 18, 18, hNumDC, (dwSecond / 60) * 6, 7, 6, 6, RGB(0, 248, 0)); // 분
	TransparentBlt(hBackbuffer, 609, 27, 18, 18, hNumDC, (dwSecond % 60) / 10 * 6, 7, 6, 6, RGB(0, 248, 0)); // 초(십)
	TransparentBlt(hBackbuffer, 634, 27, 18, 18, hNumDC, (dwSecond % 60) % 10 * 6, 7, 6, 6, RGB(0, 248, 0)); // 초(일)

	if (bFadeIn || bFadeOut) {
		AlphaBlend(hBackbuffer, 0, 0, 1216, 896, hFadeDC, 0, 0, 1216, 896, BlendFunction);
	}
}