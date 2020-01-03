#pragma once
#include<Windows.h>
#include"CGameFrame.h"

class CGameScene : public CGameFrame {
private:
	HDC hSceneDC;

	STORY_STEP storyStep;

	DWORD dwCurTime;
	DWORD dwObjectTime;
	DWORD dwSecond;
	DWORD dwSecondTime;
	DWORD dwScene;

	float fObjectXPos;
	float fObjectYPos;
public:
	void CheckKey();
	void ObjectMove();

	STORY_STEP Update();
	void Render();

	CGameScene(HWND hWnd, HDC hBackbuffer);
	~CGameScene();
};