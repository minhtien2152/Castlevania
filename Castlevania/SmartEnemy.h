#pragma once
#include "GameObject.h"
class SmartEnemy:public CGameObject
{
protected:
	LPGAMEOBJECT player;
	DWORD timeAccumulated;
public:
	SmartEnemy();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)=0;
	virtual void AdjustDirectionToFollowSimon();
};

