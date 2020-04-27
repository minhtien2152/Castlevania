#pragma once

#include "Game.h"
#include "Scene.h"
#include "GameObject.h"

#include "Simon.h"

#include "Item.h"

#include "Effect.h"
#include "Map.h"
#include "Portal.h"
class CPlayScene : public CScene
{
protected:
	Simon* player;					// A play scene has to have player, right? 

	
	


	vector<LPGAMEOBJECT> objectList;
	vector<Item*> itemList;
	vector<LPGAMEOBJECT> enemyList;
	vector<Effect*> effectList;
	Map* tileMap;
	unordered_map<int,CWeapon*> weaponList;

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadScene();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SpawnItem(LPGAMEOBJECT obj);
	void CreateEffect(LPGAMEOBJECT obj,int type);
	void CheckForWeaponCollision();
	void CheckForEnemyCollison();
	void CheckForCollisonWithItems();
	void SetSubWeapon(int type);
	void CleanUpObjects();
	void GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& coObjects );
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

