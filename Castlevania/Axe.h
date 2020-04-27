
#pragma once
#include "Weapon.h"
class Axe:public CWeapon
{
public:
	Axe();
	~Axe();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Attack(LPGAMEOBJECT user);
};

