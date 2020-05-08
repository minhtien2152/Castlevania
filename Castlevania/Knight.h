#pragma once
#include "GameObject.h"

class Knight:public CGameObject
{
public:
	Knight();
	~Knight();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

