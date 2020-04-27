#pragma once
#include "Weapon.h"
class Dagger:public CWeapon
{
public:
	Dagger();
	~Dagger();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Attack(LPGAMEOBJECT user);
	virtual void Render();
};

