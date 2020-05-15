#include "SceneMangager.h"
#include "Game.h"
#include "IntroScene.h"
#include "PlayScene.h"
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

	if (currentScene != NULL )	
	{
		if (currentScene->GetId() != INTRO_SCENE_ID)
			((CPlayScene*)currentScene)->BackUpData();
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
		currentScene = new CPlayScene(idScene, path);
	}
	game->SetKeyHandler(currentScene->GetKeyEventHandler());
	currentScene->LoadScene();
}

void CSceneMangager::ClearCurrentScene()
{
	currentScene->Unload();
	free(currentScene);
	currentScene = NULL;
}
