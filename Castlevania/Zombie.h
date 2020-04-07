#pragma once
#include "GameObject.h"
#include "Ground.h"
#define ZOMBIE_WALKING_SPEED		0.15f
#define ZOMBIE_GRAVITY				0.002f
#define ZOMBIE_MAX_DISTANCE			1200.0f

#define ZOMBIE_BBOX_WIDTH			32
#define ZOMBIE_BBOX_HEIGHT			64


#define ZOMBIE_STATE_WALKING 0
#define ZOMBIE_STATE_DIE 1



class CZombie : public CGameObject
{
	
public:
	CZombie();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};