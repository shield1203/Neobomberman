#include"CSoundManager.h"

CSoundManager* CSoundManager::Inst = nullptr;

CSoundManager* CSoundManager::getInstance() {
	if (Inst == nullptr) {
		Inst = new CSoundManager();
	}

	return Inst;
}

CSoundManager::CSoundManager() {
	System_Create(&pSystem);
	pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	// BGM
	pSystem->createSound("Resource\\Sound\\Intro_BGM.mp3", FMOD_LOOP_NORMAL, 0, &pIntro_BGM);
	pSystem->createSound("Resource\\Sound\\Selection_BGM.mp3", FMOD_LOOP_NORMAL, 0, &pSelect_BGM);
	pSystem->createSound("Resource\\Sound\\BattleMap1_BGM.mp3", FMOD_LOOP_NORMAL, 0, &p1_4BattleMap_BGM);
	pSystem->createSound("Resource\\Sound\\BattleMap2_BGM.mp3", FMOD_LOOP_NORMAL, 0, &p2_3BattleMap_BGM);
	pSystem->createSound("Resource\\Sound\\BattleResult_BGM.mp3", FMOD_LOOP_NORMAL, 0, &pBattleResult_BGM);
	pSystem->createSound("Resource\\Sound\\GameScene_BGM.mp3", FMOD_LOOP_NORMAL, 0, &pGameScene_BGM);
	pSystem->createSound("Resource\\Sound\\Stage1_BGM.mp3", FMOD_LOOP_NORMAL, 0, &pStage1_BGM);
	pSystem->createSound("Resource\\Sound\\Stage_Clear.mp3", FMOD_LOOP_NORMAL, 0, &StageClear_BGM);

	// È¿°úÀ½
	pSystem->createSound("Resource\\Sound\\Coin.wav", FMOD_DEFAULT, 0, &pCoin);
	pSystem->createSound("Resource\\Sound\\Button.wav", FMOD_DEFAULT, 0, &pButton);
	pSystem->createSound("Resource\\Sound\\Select.wav", FMOD_DEFAULT, 0, &pSelect);
	pSystem->createSound("Resource\\Sound\\Item.wav", FMOD_DEFAULT, 0, &pItem);
	pSystem->createSound("Resource\\Sound\\Walk.wav", FMOD_DEFAULT, 0, &pWalk);
	pSystem->createSound("Resource\\Sound\\Ready.wav", FMOD_DEFAULT, 0, &pReady);
	pSystem->createSound("Resource\\Sound\\SetBomb.wav", FMOD_DEFAULT, 0, &pSetBomb);
	pSystem->createSound("Resource\\Sound\\Win.mp3", FMOD_DEFAULT, 0, &pWin);
	pSystem->createSound("Resource\\Sound\\Boom.wav", FMOD_DEFAULT, 0, &pBoom);
}

CSoundManager::~CSoundManager() {
	pChBGM->stop();
	pChEffect->stop();

	pIntro_BGM->release();
	pSelect_BGM->release();
	p1_4BattleMap_BGM->release();
	p2_3BattleMap_BGM->release();
	pBattleResult_BGM->release();
	pGameScene_BGM->release();
	pStage1_BGM->release();
	StageClear_BGM->release();

	pCoin->release();
	pButton->release();
	pSelect->release();
	pItem->release();
	pWalk->release();
	pReady->release();
	pSetBomb->release();
	pWin->release();
	pBoom->release();

	pSystem->close();
	pSystem->release();
}

void CSoundManager::StartChBGM(SOUDN_BGM BGM) {
	switch (BGM)
	{
		case INTRO_BGM:
			pSystem->playSound(pIntro_BGM, 0, false, &pChBGM);
			break;
		case SELECT_BGM:
			pSystem->playSound(pSelect_BGM, 0, false, &pChBGM);
			break;
		case BATTLE_MAP1_BGM:
			pSystem->playSound(p1_4BattleMap_BGM, 0, false, &pChBGM);
			break;
		case BATTLE_MAP2_BGM:
			pSystem->playSound(p2_3BattleMap_BGM, 0, false, &pChBGM);
			break;
		case BATTLE_RESULT_BGM:
			pSystem->playSound(pBattleResult_BGM, 0, false, &pChBGM);
			break;
		case GAME_SCENE_BGM:
			pSystem->playSound(pGameScene_BGM, 0, false, &pChBGM);
			break;
		case STAGE1_BGM:
			pSystem->playSound(pStage1_BGM, 0, false, &pChBGM);
			break;
		case STAGE_CLEAR_BGM:
			pSystem->playSound(StageClear_BGM, 0, false, &pChBGM);
			break;
		}
}

void CSoundManager::StopChBGM() {
	pChBGM->stop();
}

void CSoundManager::StartChEffect(SOUND_EFFRCT Sound) {
	switch (Sound)
	{
		case SOUND_COIN:
			pSystem->playSound(pCoin, 0, false, &pChEffect);
			break;
		case SOUND_BUTTON:
			pSystem->playSound(pButton, 0, false, &pChEffect);
			break;
		case SOUND_SELECT:
			pSystem->playSound(pSelect, 0, false, &pChEffect);
			break;
		case SOUND_ITEM:
			pChEffect->setVolume(1);
			pSystem->playSound(pItem, 0, false, &pChEffect);
			break;
		case SOUND_BOOM:
			pSystem->playSound(pBoom, 0, false, &pChEffect);
			break;
	}
}

void CSoundManager::StopChEffect() {
	pChEffect->stop();
}

void CSoundManager::StartSubChEffect(SOUND_EFFRCT Sound) {
	switch (Sound)
	{
		case SOUND_WALK:
			pSystem->playSound(pWalk, 0, false, &pSubChEffect);
			break;
		case SOUND_SET_BOMB:
			pSystem->playSound(pSetBomb, 0, false, &pSubChEffect);
			break;
	}
}

void CSoundManager::StopSubChEffect() {
	pSubChEffect->stop();
}

void CSoundManager::StartChAnnouncer(SOUND_ANNOUNCER Sound) {
	switch (Sound)
	{
		case SOUND_WIN:
			pSystem->playSound(pWin, 0, false, &pChAnnouncer);
			break;
		case SOUND_READY:
			pSystem->playSound(pReady, 0, false, &pChAnnouncer);
			break;
	}
}

void CSoundManager::StopChAnnouncer() {
	pChAnnouncer->stop();
}