#pragma once
#include"SmartEnemy.h"
class Raven: public SmartEnemy
{
	float destinationX;
	float destinationY;
	bool hasDoneFirstPositioning;

public:
	Raven();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	void SetDesToFollowSimon();
	bool IsPlayerWithinAttackRange();
	void PickRandomPositioning();
	void SetChargingSpeed();
	bool IsAtDestination();
};

