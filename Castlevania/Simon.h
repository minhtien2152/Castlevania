#pragma once
#include"GameObject.h"
#include "Whip.h"

#define SIMON_WALKING_SPEED		0.2f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_GRAVITY			0.0022f
#define SIMON_DEFLECT_SPEED_X	0.05
#define SIMON_DEFLECT_SPEED_Y	0.5
#define SIMON_STAIR_SPEED		0.075
#define SIMON_INVULNERABLE_TIME 3000
#define SIMON_INVISIBILITY_TIME 4000
#define SIMON_STAND				0
#define SIMON_WALK				1
#define SIMON_SIT				2
#define SIMON_JUMP				3
#define SIMON_SIT_ATTACK		4
#define SIMON_STAND_ATTACK		5
#define SIMON_POWERUP			6
#define SIMON_STAIR_UP			7	
#define SIMON_STAIR_UP_IDLE		8

#define SIMON_STAIR_DOWN		9
#define SIMON_STAIR_DOWN_IDLE	10
#define SIMON_STAIR_UP_ATK		11
#define SIMON_STAIR_DOWN_ATK	12
#define SIMON_DAMAGED			13
#define SIMON_DEAD				14
#define SIMON_WAIT				15

#define STAIR_STATE_GOING_UP	-1
#define	STAIR_STATE_GOING_DOWN	1
#define	STAIR_STATE_NONE		0

class Simon : public CGameObject
{
	int score;
	int heart;
	int life;
	

	DWORD invulnerable_start;
	DWORD invisibility_start;
	Whip* mainWeapon;
	CWeapon* subWeapon;
	int currentStairDirection;
	int currentStairType;
	int currentStairId;
	float stairEnterX;
public:
	Simon();
	~Simon();
	bool isJumping = false;
	bool isSitting = false;
	bool isInvulnerable = false;
	bool isInvisible = false;
	bool canUseSubWeapon = false;
	bool IsAttacking();
	bool isWaitingForAni = false;
	int stairState;
	bool isAllowToGoUp;
	bool isAllowToGoDown;
	bool isCollidingStairObject;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);
	void PullUp();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void AttackWithWhip();
	void AttackWithSubWeapon();
	Whip* GetMainWeapon();
	int GetScore() { return score; }
	void SetScore(int score);
	int GetHeartsCollected() { return heart; }
	void SetHeartsCollected(int heartNum);
	int GetLife() { return life; }
	void SetLife(int life);
	void StartInvisibilityTimer();
	void SetSubWeapon(CWeapon * wp);
	CWeapon* GetSubWeapon();
	int GetCurrentStairDirection();
	int GetCurrentStairType();
	float GetStairEnterPosX();
	void GoToStairEnterPos();
	void GoIntoStair(int upOrDown,int direction);

};

