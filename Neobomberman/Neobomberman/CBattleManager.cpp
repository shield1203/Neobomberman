#include"CBattleManager.h"
#include"CMapSelect.h"
#include"CBomberSelect.h"
#include"CBattle.h"
#include"CBattleResult.h"

CBattleManager* CBattleManager::inst = nullptr;

CBattleManager* CBattleManager::getInstance(HWND hWnd, HDC hBackbuffer) {
	if (inst == nullptr) {
		inst = new CBattleManager(hWnd, hBackbuffer);
	}

	return inst;
}

CBattleManager::CBattleManager(HWND hWnd, HDC hBackbuffer) {
	this->hWnd = hWnd;
	this->hBackbuffer = hBackbuffer;

	Init();
}

CBattleManager::~CBattleManager() {
	delete MapSelect;
	delete BomberSelect;
	delete Battle;
	delete BattleResult;
}

void CBattleManager::Init() {
	gameStep = STEP_BATTLE;
	battleStep = BATTLE_MAP_SELECT;

	MapSelect = new CMapSelect(hWnd, hBackbuffer);
	BomberSelect = nullptr; // new CBomberSelect(hWnd, hBackbuffer);
	Battle = nullptr; //new CBattle(hWnd, hBackbuffer);
	BattleResult = nullptr; // new CBattleResult(hWnd, hBackbuffer);
}

GAME_STEP CBattleManager::Update() {
	switch (battleStep) {
		case BATTLE_MAP_SELECT: {
			if (MapSelect == nullptr) {
				MapSelect = new CMapSelect(hWnd, hBackbuffer);
			}
			MapSelect->Update();
			break;
		}
		case BATTLE_BOMBER_SELECT: {
			BomberSelect->Update();
			break;
		}
		case BATTLE_PLAY: {
			battleStep = Battle->Update();
			break;
		}
		case BATTLE_RESULT: {
			battleStep = BattleResult->Update();
			break;
		}
	}

	if (gameStep == STEP_INTRO) {
		delete MapSelect;
		delete BomberSelect;
		delete Battle;
		delete BattleResult;
		return gameStep;
	}

	switch (battleStep) {
		case BATTLE_BOMBER_SELECT: {
			if (BomberSelect == nullptr) {
				BomberSelect = new CBomberSelect(hWnd, hBackbuffer);
				BomberSelect->Init();
			}

			if (!BomberSelect->bOn) {
				//BomberSelect->Init(asd);
			}
			break;
		}
		case BATTLE_PLAY: {
			if (Battle == nullptr) {
				Battle = new CBattle(hWnd, hBackbuffer);
			}

			if (!Battle->bOn) {
				Battle->Init();
			}
			break;
		}
		case BATTLE_RESULT: {
			if (BattleResult == nullptr) {
				BattleResult = new CBattleResult(hWnd, hBackbuffer);
			}

			if (!BattleResult->bOn) {
				if (Battle->bomber[0].state != P_STATE_DIE) {
					BattleResult->Init(Battle->bomber[0].player);
				}
				else {
					BattleResult->Init(Battle->bomber[1].player);
				}
			}
			break;
		}
	}

	return gameStep;
}

void CBattleManager::Render() {
	switch (battleStep) {
		case BATTLE_MAP_SELECT: {
			MapSelect->Render();
			break;
		}
		case BATTLE_BOMBER_SELECT: {
			BomberSelect->Render();
			break;
		}
		case BATTLE_PLAY: {
			Battle->Render();
			break;
		}
		case BATTLE_RESULT: {
			BattleResult->Render();
			break;
		}
	}
}