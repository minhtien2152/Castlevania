#pragma once
#include "SmartEnemy.h"
class Ghost:public SmartEnemy
{
public:
	Ghost();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void AdjustDirectionToFollowSimon();
};

