#pragma once
#include "GameObject.h"
class Bumper: public CGameObject
{

public:
	Bumper();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
};

