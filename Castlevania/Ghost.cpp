#include "Ghost.h"
#include "Simon.h"
#include "Define.h"
#define GHOST_SPEED 0.07f
#define GHOST_SPEED_VY 0.1f

Ghost::Ghost() : SmartEnemy()
{
	isPhysicEnabled = false;
	isActivated = false;
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	if (!isActivated)
	{
		isActivated = CheckActivate();
		x -= dx;
		y -= dy;
	}
	else
	{
		
		AdjustDirectionToFollowSimon();
		if (GetTickCount() - lastGlide >= 500)
		{
			SetChargingSpeed();
		}
		
		if (timeAccumulated >= 2000)
		{
			timeAccumulated = 0;
			if (rand() % 2 == 1)
			{
				if (rand() % 2 == 1)
					vy += GHOST_SPEED;
				else
					vy -= GHOST_SPEED;
				lastGlide = GetTickCount();
			}
		}
	}
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 32;
}

void Ghost::Render()
{
	//if(isActivated)
		animation_set->at(state)->Render(x, y, nx);
}



void Ghost::SetChargingSpeed()
{
	float plr_x, plr_y;
	player->GetPosition(plr_x, plr_y);
	D3DXVECTOR2* v1 = new D3DXVECTOR2(plr_x - x, plr_y - y);
	D3DXVECTOR2* v2 = new D3DXVECTOR2(plr_x - x, 0);
	/*D3DXVec2Normalize(v1, v1);
	D3DXVec2Normalize(v2, v2);*/
	float angle = acos(D3DXVec2Dot(v1, v2) / (D3DXVec2Length(v1) * D3DXVec2Length(v2)));
	float temp_vx = GHOST_SPEED * cos(angle);
	float temp_vy = GHOST_SPEED * sin(angle);

	if (plr_x - x < 0)
		vx = -temp_vx;
	else
		vx = temp_vx;
	if (plr_y - y < 0)
		vy = -temp_vy;
	else
		vy = temp_vy;

}

bool Ghost::CheckActivate()
{
	if (player != NULL)
	{
		float plr_x = player->GetPosX();
		if (((x - plr_x) / abs(x - plr_x)) == nx && abs(x - plr_x) >= TILE_WIDTH*4)
			return true;
	}
	return false;
}
