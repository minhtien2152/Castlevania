#pragma once

#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include "Ground.h"
#include "Zombie.h"
#include "Simon.h"
#include "Candle.h"
#include "Item.h"
#include "Textures.h"
#include "Animations.h"
#include "Effect.h"
#include "StatusBoard.h"
#include "Map.h"
class CPlayScene : public CScene
{
protected:
	Simon* player;					// A play scene has to have player, right? 

	StatusBoard* statusboard;
	

	vector<LPGAMEOBJECT> objectList;
	vector<Item*> itemList;
	vector<LPGAMEOBJECT> enemyList;
	vector<LPEFFECT> effectList;
	Map* tileMap;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_RESOURCES(string line);
	void _ParseSection_TILEMAP_DATA(string line);
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadScene();
	virtual void LoadObjectResourses(LPCWSTR filepath);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SpawnItem(LPGAMEOBJECT obj);
	void CreateEffect(LPGAMEOBJECT obj);
	void CheckForWeaponCollision();
	void CheckForEnemyCollison();
	void CheckForCollisonWithItems();
	void GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& coObjects);
	//void AqquireWeapon(Weapon_Type type);
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

