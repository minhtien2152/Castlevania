#pragma once
#include"SmartEnemy.h"
class Bat : public SmartEnemy
{
	float a;
	float default_x, default_y;
public:
	Bat();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	virtual void SetPosition(float x, float y);
	void CalculateFlyingEquation();
	bool IsPlayerWithinAttackRange();
};