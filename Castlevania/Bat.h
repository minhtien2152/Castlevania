#pragma once
#include "GameObject.h"

class Bat : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();


public:
	Bat();
	virtual void SetState(int state);
};