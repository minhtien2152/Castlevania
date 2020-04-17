#pragma once
#include "GameObject.h"
#include "Define.h"
class SubWeapon : public CGameObject
{


public:
	SubWeapon();
	~SubWeapon();
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects = NULL, vector<LPGAMEOBJECT>* dynamicCoObjects = NULL);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	void Attack();
};

