#pragma once
#include "SmartEnemy.h"
class HunchBack:public SmartEnemy
{
	int duplicateState;
public:
	HunchBack();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	void RandomizeState();
};

