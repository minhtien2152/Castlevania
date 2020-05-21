#pragma once
#include "Axe.h"
class Bone: public Axe
{
public:
	Bone();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

