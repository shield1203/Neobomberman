#pragma once
#include"Define.h"
#include"inc/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

using namespace FMOD;

class CSoundManager {
private:
	static CSoundManager* Inst;
	CSoundManager();

	System* pSystem;
	Sound* pIntro_BGM, * pSelect_BGM, *p1_4BattleMap_BGM, * p2_3BattleMap_BGM, *pBattleResult_BGM, *pGameScene_BGM, *pStage1_BGM, * StageClear_BGM;
	Sound* pCoin, * pButton, *pSelect, *pItem, *pWalk, *pSetBomb, *pBoom;
	Sound* pReady, * pWin;
	Channel* pChBGM, * pChEffect, *pSubChEffect, *pChAnnouncer;
public:
	static CSoundManager* getInstance();

	void StartChBGM(SOUDN_BGM BGM);
	void StopChBGM();
	void StartChEffect(SOUND_EFFRCT Sound);
	void StopChEffect();
	void StartSubChEffect(SOUND_EFFRCT Sound);
	void StopSubChEffect();
	void StartChAnnouncer(SOUND_ANNOUNCER Sound);
	void StopChAnnouncer();

	~CSoundManager();
};