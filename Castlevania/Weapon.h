#pragma once
#include "GameObject.h"
#include "Utils.h"
class CWeapon : public CGameObject
{
protected:
	int damage;
public:
	CWeapon();
	~CWeapon();
	virtual void Attack(LPGAMEOBJECT user);
	virtual void SetPosition(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL)=0;
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)=0;
	
};

