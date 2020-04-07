#pragma once
#include"GameObject.h"
#include "Whip.h"
#define SIMON_WALKING_SPEED		0.2f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_GRAVITY			0.002f
#define SIMON_DEFLECT_SPEED_X	0.05
#define SIMON_DEFLECT_SPEED_Y	0.5
#define SIMON_INVULNERABLE_TIME 10000

#define SIMON_STAND				0
#define SIMON_WALK				1
#define SIMON_SIT				2
#define SIMON_JUMP				3
#define SIMON_SIT_ATTACK		4
#define SIMON_STAND_ATTACK		5
#define simon_powerup			6
#define simon_stair_up			7	
#define simon_stair_down		8
#define simon_stair_up_atk		9
#define simon_stair_down_atk	10
#define SIMON_DEFLECT			11
class Simon : public CGameObject
{
	DWORD invulnerable_start;
	Whip* mainWeapon;
public:
	Simon();
	~Simon();
	bool isJumping = false;
	bool isSitting = false;
	bool isInvulerable = false;
	bool IsAttacking();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void PullUp();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Attack();
};

