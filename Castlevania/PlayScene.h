#pragma once

#include "Game.h"
#include "Scene.h"
#include "GameObject.h"

#include "Simon.h"

#include "Item.h"

#include "Effect.h"
#include "Map.h"
#include "Portal.h"
#include "Grid.h"
class CPlayScene : public CScene
{
protected:
						
	int id_counter;
	int prev_scene;
	Grid* static_obj_grid;
	Grid* dynamic_obj_grid;

	DWORD deathTimer;


	vector<LPGAMEOBJECT> objectList;
	vector<Item*> itemList;
	vector<LPGAMEOBJECT> enemyList;
	vector<Effect*> effectList;
	Map* tileMap;
	vector<CWeapon*> weaponList;
	vector<CWeapon*> enemyProjectTile;
	int weaponWaitingToBeProcess;

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);
public:
	CPlayScene(int id, LPCWSTR filePath, int preScene =0);

	virtual void LoadScene();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SpawnItem(LPGAMEOBJECT obj);
	void CreateEffect(LPGAMEOBJECT obj,int type);

	void CheckForEnemyCollison();
	void CheckForCollisonWithItems();
	void AccquireItem(int type);
	void CleanUpObjects();
	void GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& coObjects );
	void BackUpData();
	void LoadBackUpData();
	void UpdateListsAccordingGrid();
	void SetWeaponToBeProcess(int type);
	void CreateSubWeapon(int type);
	void CheckSimonDead();
	bool IsStopWatchEnabled();
	int GetWeaponNum();
	void DestroyAllOnScreenEnemy();
	friend class CPlayScenceKeyHandler;

};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

