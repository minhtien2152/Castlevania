#pragma once
#include"GameObject.h"

#define SIMON_WALKING_SPEED		0.12f
#define SIMON_JUMP_SPEED_Y		0.6f
#define SIMON_GRAVITY			0.002f



#define SIMON_STAND				0
#define SIMON_WALK				1
#define SIMON_SIT				2
#define SIMON_JUMP				3

#define SIMON_SIT_ATTACK		4
#define SIMON_STAND_ATTACK		5

class Simon : public CGameObject
{
public:
	bool isJumping = false;
	bool isSitting = false;
	bool IsAttacking();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

