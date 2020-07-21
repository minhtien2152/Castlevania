#pragma once
#include "Weapon.h"
class FireBall:public CWeapon
{
public:
	FireBall();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetSpeed(float desX, float desY);
};

