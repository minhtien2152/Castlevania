#include "SceneMangager.h"
#include "Game.h"
#include "IntroScene.h"
#include "Define.h"
#include "BackUp.h"


CSceneMangager* CSceneMangager::__instance = NULL;

void CSceneMangager::Add(int id, LPCWSTR path)
{
	scene_database[id] = path;
}
CSceneMangager* CSceneMangager::GetInstance()
{
	if (__instance == NULL) __instance = new CSceneMangager();
	return __instance;
}

LPSCENE CSceneMangager::GetCurrentScene()
{
	return currentScene;
}

void CSceneMangager::SwitchScene(int idScene)
{
	CGame* game = CGame::GetInstance();
	int prevScene=-1;
	if (currentScene != NULL )	
	{
		currentScene->IsFinished = true;
		if (currentScene->GetId() != INTRO_SCENE_ID)
			if (!BackUp::GetInstance()->HaveJustLostLife())
				((CPlayScene*)currentScene)->BackUpData();
		prevScene = currentScene->GetId();
		ClearCurrentScene();
	}
	LPCWSTR path = scene_database[idScene];
	if (idScene == INTRO_SCENE_ID)
	{
		BackUp::GetInstance()->ResetData();
		game->SetState(GAME_STATE_MENU);
		currentScene = new CIntroScene(idScene,path);
	}
	else
	{
		game->SetState(GAME_STATE_PLAYSCENE);
		currentScene = new CPlayScene(idScene, path,prevScene);
	}
	game->SetKeyHandler(currentScene->GetKeyEventHandler());
	currentScene->LoadScene();
}

void CSceneMangager::ResetScene()
{
	SwitchScene(currentScene->GetId());
}

void CSceneMangager::ClearCurrentScene()
{
	currentScene->Unload();
	free(currentScene);
	currentScene = NULL;
}
