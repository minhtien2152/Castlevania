#pragma once
#include"Weapon.h"

#define WHIP_LEVEL0 0
#define WHIP_LEVEL1 1
#define WHIP_LEVEL2 2

class Whip : public CWeapon
{
public:
	Whip();
	~Whip();
	virtual void SetPosition(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void LevelUp();
};
