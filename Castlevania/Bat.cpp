#include "Bat.h"
#include "Define.h"

Bat::Bat()
{
	a = 0;
	isPhysicEnabled = false;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);

	if (state == BAT_IDLE)
	{

		if (IsPlayerWithinAttackRange())
		{
			AdjustDirectionToFollowSimon();
			CalculateFlyingEquation();
			SetState(BAT_FLY);
			
		}
	}
	
	if (state == BAT_FLY)
	{
		//the flying equation is a parabol x = a(y-y0)^2 +x0
		y = sqrt((x - default_x) / a) + default_y;	
	}
}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 32;
}

void Bat::SetState(int state)
{
	SmartEnemy::SetState(state);
	switch (state)
	{
	case BAT_IDLE:
		break;
	case BAT_FLY:
		vx = -nx * BAT_SPEED_VX;
		vy = 0;
		break;
	default:
		break;
	}
}

void Bat::SetPosition(float x, float y)
{
	this->x = default_x = x;
	this->y = default_y = y;

}

void Bat::CalculateFlyingEquation()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		a = (plr_x - default_x) / ((plr_y - default_y) * (plr_y - default_y));
	}
}

bool Bat::IsPlayerWithinAttackRange()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		if (abs(plr_x - x) <= TILE_WIDTH * 6 && abs(plr_y - y) <= TILE_WIDTH * 4)
			return true;
	}
	return false;
}
