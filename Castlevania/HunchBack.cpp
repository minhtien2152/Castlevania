#include "HunchBack.h"
#include "Define.h"
#include "Simon.h"

HunchBack::HunchBack() : SmartEnemy()
{
	
	duplicateState = 0;
}
void HunchBack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	vy += GRAVITY * dt;
	if (isTouchingGround )
	{
		if (state == HUNCHBACK_IDLE && !animation_set->at(state)->IsOver())
			return;
		if (timeAccumulated >= 500)
		{
			AdjustDirectionToFollowSimon();
			timeAccumulated = 0;
		}
		RandomizeState();	
	}
}

void HunchBack::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 31;
}

void HunchBack::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HUNCHBACK_IDLE:
		ResetAni();
		vx = 0;
		vy = 0;
		
		break;
	case HUNCHBACK_JUMP:
		vx = -nx*HUNCHBACK_SPEED_VX;
		if (rand() % 2 == 0)
			vy = HUNCHBACK_SPEED_JUMP_VY;
		else
			vy = HUNCHBACK_SPEED_CHARGE_VY;
		break;
	default:
		break;
	}
}

void HunchBack::RandomizeState()
{
	int randState = rand() % 2;
	if (randState == state)
		duplicateState++;
	else
		duplicateState = 0;
	if (duplicateState >= 3)
	{
		duplicateState = 0;
		if (randState == 1)
			SetState(HUNCHBACK_IDLE);
		else
			SetState(HUNCHBACK_JUMP);
	}
	else SetState(randState);
}
