#include "Ghost.h"
#include "Simon.h"
#define GHOST_SPEED_VY 0.07f
#define GHOST_SPEED_VX 0.07f

Ghost::Ghost() : SmartEnemy()
{
	isPhysicEnabled = false;
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	x += dx;
	y += dy;
	timeAccumulated += dt;
	vx = -nx*GHOST_SPEED_VX;
	if (vy == 0)
		if (rand() % 2 == 0)
			if (rand() % 2 == 0)
				vy += GHOST_SPEED_VY;
			else
				vy -= GHOST_SPEED_VY;
	
	if (timeAccumulated >= 1000)
	{
		timeAccumulated = 0;
		AdjustDirectionToFollowSimon();
	}
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 32;
}

void Ghost::AdjustDirectionToFollowSimon()
{
	SmartEnemy::AdjustDirectionToFollowSimon();
	float plr_x, plr_y;
	player->GetPosition(plr_x, plr_y);
	int randNum = rand() % 20+1;
	if (plr_y +16 - y>2)
		vy = GHOST_SPEED_VX*randNum/10;
	else if (plr_y +16  - y < 2)
		vy = -GHOST_SPEED_VX*randNum / 10;
	else
		vy = 0;
}
