#pragma once
#include<Windows.h>
#include"Define.h"
#include"CGameScene.h"
#include"CStory.h"

class CStoryManager {
private:
	static CStoryManager* inst;

	CStoryManager(HWND hWnd, HDC hBackbuffer);
public:
	static CStoryManager* getInstance(HWND hWnd, HDC hBackbuffer);

	HWND hWnd;
	HDC hBackbuffer;

	GAME_STEP gameStep;
	STORY_STEP storyStep;

	CGameScene* GameScene;
	CStory* Story;

	void Init();
	GAME_STEP Update();
	void Render();

	~CStoryManager();
};