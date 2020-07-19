#pragma once
#include"GameObject.h"
#include "StairBottom.h"
#include "Whip.h"


class Simon : public CGameObject
{
	int score;
	int heart;
	int life;
	

	DWORD invulnerable_start;
	Whip* mainWeapon;

	int subWeaponType;
	int subWeaponMaxCap;


	//==all things stair related==
	int currentStairDirection;
	int currentStairType;

	float stairEnterX;
	vector<LPGAMEOBJECT> stairObjects;
	int stateWaitingToBeRendered;
	//=============================
public:
	Simon();
	~Simon();
	bool isJumping = false;
	bool isSitting = false;
	bool isInvulnerable = false;
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

	Whip* GetMainWeapon();
	int GetScore() { return score; }
	void SetScore(int score);
	int GetHeartsCollected() { return heart; }
	void SetHeartsCollected(int heartNum);
	int GetLife() { return life; }
	void SetLife(int life);
	void StartInvisibilityTimer();

	void SetSubWeaponItem(int type);
	int GetSubWeaponType();
	void SetSupWeaponCap(int capacity);
	int GetSubWeaponCap();
	bool IsInRightFrameToUseSubWeapon();

	int GetCurrentStairDirection();
	int GetCurrentStairType();
	void SetCurrentStairDirection(int dir);
	void SetCurrentStairType(int type);

	float GetStairEnterPosX();
	void GoToStairEnterPos();
	void GoIntoStair(int upOrDown,int direction);
	void ProcessStair(int type);
	void CheckGetOffStair();
	void TakeDamage(int damage);
	void SetStateToBeRender(int state);
};

