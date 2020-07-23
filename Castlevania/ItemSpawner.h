#pragma once
#include "GameObject.h"

class ItemSpawner:public CGameObject
{
	
	vector<LPGAMEOBJECT>* itemList;
	float spawn_posX;
	float spawn_posY;
public:
	ItemSpawner();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void SetItemList(vector<LPGAMEOBJECT>* list);
	void SetItemSpawnPoint(float x, float y);
	void Activate();
};

