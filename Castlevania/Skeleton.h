#pragma once
#include "SmartEnemy.h"
class Skeleton:public SmartEnemy
{
	float x_backUp;
	int nx_backUp;
public:
	Skeleton();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	virtual void AdjustDirectionToFollowSimon();
};

