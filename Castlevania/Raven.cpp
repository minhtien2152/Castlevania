#include "Raven.h"
#include "Define.h"
#include "Utils.h"
#include <DirectXMath.h>

Raven::Raven()
{
	isPhysicEnabled = false;
	state = RAVEN_IDLE;
	isDestructable = true;
	destinationX = destinationY = 0;

}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	SmartEnemy::Update(dt, coObjects);
	AdjustDirectionToFollowSimon();
	if (destinationX == 0 && destinationY == 0)
	{
		PickRandomPositioning();
		
	}

 	if(state == RAVEN_IDLE)
		if(!isFrozen)
		if (IsPlayerWithinAttackRange())
		{
			SetState(RAVEN_FLY);
			SetChargingSpeed();
		}

	if (state == RAVEN_FLY)
	{
		
		if (IsAtDestination())
		{
			vx = vy = 0;
			if (timeAccumulated >= RAVEN_WAIT_TIME)
			{
				SetDesToFollowSimon();
				SetChargingSpeed();

			}
		}
		else
			timeAccumulated = 0;
		
	}
	//DebugOut(L"x %f, y %f\n", x, y);
	//DebugOut(L"desX %f, desY %f\n", destinationX, destinationY);

}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 32;
}

void Raven::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RAVEN_IDLE:
		vx = vy = 0;
		break;
	case RAVEN_FLY:
		break;
	default:
		break;
	}
}



void Raven::SetDesToFollowSimon()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		destinationX = plr_x + TILE_WIDTH;
		destinationY = plr_y + TILE_HEIGHT/2;
	}
}

bool Raven::IsPlayerWithinAttackRange()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		if (abs(plr_x - x) <= TILE_WIDTH * 6)
			return true;
	}
	return false;
}

void Raven::PickRandomPositioning()
{
	
 	int randX = rand() % (TILE_WIDTH*2);
	int randY = rand() % (TILE_WIDTH*2);
	if (rand() % 2 == 1)
		randX *= -1;
	if (rand() % 2 == 1)
		randY *= -1;
	destinationX = x + randX;
  	destinationY = y + randY;
}

void Raven::SetChargingSpeed()
{
	D3DXVECTOR2* v1 = new D3DXVECTOR2(destinationX - x, destinationY  - y);
	D3DXVECTOR2* v2 = new D3DXVECTOR2(destinationX  - x, 0);
	/*D3DXVec2Normalize(v1, v1);
	D3DXVec2Normalize(v2, v2);*/
	float angle = acos(D3DXVec2Dot(v1, v2) / (D3DXVec2Length(v1) * D3DXVec2Length(v2)));
	float temp_vx = RAVEN_SPEED * cos(angle);
	float temp_vy = RAVEN_SPEED * sin(angle);

	if (destinationX - x < 0)
		vx = -temp_vx;
	else
		vx = temp_vx;
	if (destinationY - y < 0)
		vy = -temp_vy;
	else
		vy = temp_vy;
	DebugOut(L"goc %f do\n", angle * 180 / PI);
	DebugOut(L"vx %f, vy %f\n", vx, vy);
}

bool Raven::IsAtDestination()
{
	if (abs(destinationX - x) <= 5 && abs(destinationY - y) <= 5)
		return true;
	return false;
}
