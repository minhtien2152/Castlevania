#include "Skeleton.h"
#include "Define.h"
#define SKELETON_MOVING	0
#define SKELETON_JUMP	1
#define SKELETON_SPEED_VX 0.1
#define SKELETON_SPEED_VY	0.4	
Skeleton::Skeleton() :SmartEnemy()
{
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	if (timeAccumulated >= 1000)
	{
		timeAccumulated = 0;
		AdjustDirectionToFollowSimon();
	}
	vy += GRAVITY * dt;
	if (isTouchingGround)
		if (x >= x_backUp + TILE_WIDTH)
			nx_backUp = 1;
		else if (x <= x_backUp - TILE_WIDTH)
			nx_backUp = -1;
	vx = -nx_backUp*SKELETON_SPEED_VX;
	
	
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y;
	right = left + 32;
	bottom = top + 63;
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKELETON_MOVING:

		break;
	case SKELETON_JUMP:
		vy = -SKELETON_SPEED_VY;
		break;
	default:
		break;
	}
}

void Skeleton::AdjustDirectionToFollowSimon()
{
	SmartEnemy::AdjustDirectionToFollowSimon();
	float plr_x, plr_y;
	player->GetPosition(plr_x, plr_y);
	if (plr_x - x > 0)
		x_backUp = plr_x - TILE_WIDTH * 6;
	else
		x_backUp = plr_x+ SIMON_BBOX_WIDTH + TILE_WIDTH * 6;
}
