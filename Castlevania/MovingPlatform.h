#pragma once
#include "GameObject.h"
class MovingPlatform: public CGameObject
{
public:
	MovingPlatform();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

