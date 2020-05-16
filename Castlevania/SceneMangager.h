#pragma once
#include "Scene.h"
#include "Simon.h"
#include "PlayScene.h"
class CSceneMangager
{
	static CSceneMangager* __instance;
	unordered_map<int, LPCWSTR> scene_database;
	LPSCENE currentScene;
public:
	 void Add(int id, LPCWSTR path);
	 static CSceneMangager* GetInstance();
	 LPSCENE GetCurrentScene();
	 void SwitchScene(int idScene);
	 void ClearCurrentScene();

};

