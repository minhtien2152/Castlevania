#pragma once

#include "Weapon.h"
class Holywater:public CWeapon
{
	DWORD brokeTime;
public:
	Holywater();
	~Holywater();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Break();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Attack(LPGAMEOBJECT user);
};

