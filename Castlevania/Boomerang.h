#pragma once
#include "Weapon.h"
class Boomerang:public CWeapon
{
public:

	Boomerang();
	~Boomerang();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Attack(LPGAMEOBJECT user);
};

