#pragma once
#include "Weapon.h"

class StopWatch:public CWeapon
{
	DWORD activateTime;
	public:
		StopWatch();
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
		virtual void Attack(LPGAMEOBJECT user);
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


};

