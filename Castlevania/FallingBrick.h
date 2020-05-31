#pragma once
#include "GameObject.h"
class FallingBrick : public CGameObject
{
	
public:
	FallingBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

