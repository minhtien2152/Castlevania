#pragma once
#include "SmartEnemy.h"
#include "Bone.h"
class Skeleton:public SmartEnemy
{
	float x_backUp;
	int nx_backUp;

	vector<CWeapon*>* projectTileList;
	DWORD lastAttack;
	DWORD lastTurnAround;
public:
	Skeleton();
	int screen_height;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	virtual void AdjustMovementCenter();
	void SetProjectTileList(vector<CWeapon*>* projectTileList);
	void ProcessWeapon();
	bool CheckJumpingCondition();
	int JumpingSimulator();
	int IsOnEdge();
	SmartEnemy* CreateDecoy(float x, float y);
	void TurnAround();
};

