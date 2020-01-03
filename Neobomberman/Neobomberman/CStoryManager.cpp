#include"CStoryManager.h"

CStoryManager* CStoryManager::inst = nullptr;

CStoryManager* CStoryManager::getInstance(HWND hWnd, HDC hBackbuffer) {
	if (inst == nullptr) {
		inst = new CStoryManager(hWnd, hBackbuffer);
	}

	return inst;
}

CStoryManager::CStoryManager(HWND hWnd, HDC hBackbuffer) {
	this->hWnd = hWnd;
	this->hBackbuffer = hBackbuffer;

	gameStep = STEP_STORY;
	storyStep = STORY_SCENE;

	GameScene = new CGameScene(hWnd, hBackbuffer);
}

CStoryManager::~CStoryManager() {
	delete GameScene;
}

void CStoryManager::Init() {
	gameStep = STEP_STORY;
	storyStep = STORY_SCENE;
}

GAME_STEP CStoryManager::Update() {
	switch (storyStep) {
		case STORY_SCENE: {
			if (GameScene == nullptr) {
				GameScene = new CGameScene(hWnd, hBackbuffer);
			}
			storyStep = GameScene->Update();
			break;
		}
		case STORY_PLAY: {
			storyStep = Story->Update();
			break;
		}
	}

	switch (storyStep) { // ½ºÅÜÀÌ ¹Ù²ğ¶§ ÇÒ´ç
		case STORY_PLAY: {
			if (Story == nullptr) {
				Story = new CStory(hWnd, hBackbuffer);
			}
			break;
		}
		case STORY_END: {
			delete GameScene;
			delete Story;
			GameScene = nullptr;
			Story = nullptr;

			return STEP_INTRO;
		}
	}

	return gameStep;
}

void CStoryManager::Render() {
	switch (storyStep) {
		case STORY_SCENE: {
			if (GameScene == nullptr) {
				GameScene = new CGameScene(hWnd, hBackbuffer);
			}
			GameScene->Render();
			break;
		}
		case STORY_PLAY: {
			Story->Render();
			break;
		}
	}
}