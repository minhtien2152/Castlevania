#include "BossBat.h"
#include "Define.h"
#include "Utils.h"
#define BOSS_SPEED 0.12f
#define BOSS_IDLE 0
#define BOSS_FLY	1
#define BOSS_WAIT_TIME 1000
#define DEFAULT_A -0.016
#define BOSS_CHARGING_TIME 600
#define BOSS_IDLE_BBOX 32
#define BOSS_FLY_BBOX_WIDTH 96
#define BOSS_FLY_BBOX_HEIGHT 46

BossBat::BossBat()
{
	isPhysicEnabled = false;
	isDestructable = true;
}

void BossBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	if (state == BOSS_IDLE)
		if (IsPlayerWithinAttackRange())
		{
			SetState(BOSS_FLY);
			cam->InBossFight();
			Hover();
		}

	if (state == BOSS_FLY)
	{
		if (isHovering)
		{
			if (IsAtDestination())
			{
				vx = vy = 0;
				if (timeAccumulated >= BOSS_WAIT_TIME)
				{
					Charge();
				}
			}
			else
			timeAccumulated = 0;
		}
		
			
		if (isCharging)
		{
			y = a * (x - destinationX) * (x - destinationX) + destinationY;
			HandleGoingOutOfScreen();
		}
		DebugOut(L"vx = %f\n", vx);
		DebugOut(L"a = %f\n",a);
	}
}

void BossBat::HandleGoingOutOfScreen()
{
	float camx, camy;
	cam->GetCamPosition(camx, camy);
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	if (r >= camx + screen_width-TILE_WIDTH/2 || l <= camx +TILE_WIDTH/2  || t  <= STATUS_BOARD_HEIGHT + TILE_HEIGHT)
	{
		Hover();
	}
}
void BossBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	right = left + BOSS_IDLE_BBOX;
	bottom = top + BOSS_IDLE_BBOX;
	if (state == BOSS_FLY)
	{
		right = left + BOSS_FLY_BBOX_WIDTH;
		bottom = top + BOSS_FLY_BBOX_HEIGHT;
	}
}

void BossBat::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_IDLE:
		vx = vy = 0;
		break;
	case BOSS_FLY:
		break;
	default:
		break;
	}
}

void BossBat::SetCam(Camera* camera)
{
	cam = camera;
}


bool BossBat::IsPlayerWithinAttackRange()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		if (plr_x - x >= TILE_WIDTH * 4)
			return true;
	}
	return false;
}

void BossBat::PickRandomPositioning()
{
	float camx, camy;
	cam->GetCamPosition(camx, camy);
	int randX = rand() % (screen_width - BOSS_FLY_BBOX_WIDTH*2) + BOSS_FLY_BBOX_WIDTH;
	int randY = rand() % (screen_height - BOSS_FLY_BBOX_HEIGHT *2)+STATUS_BOARD_HEIGHT + BOSS_FLY_BBOX_HEIGHT;

	destinationX = camx + randX;
	destinationY = camy + randY;
}

void BossBat::SetChargingSpeed()
{
	if (isHovering)
	{
		D3DXVECTOR2* v1 = new D3DXVECTOR2(destinationX - x, destinationY - y);
		D3DXVECTOR2* v2 = new D3DXVECTOR2(destinationX - x, 0);
		/*D3DXVec2Normalize(v1, v1);
		D3DXVec2Normalize(v2, v2);*/
		float angle = acos(D3DXVec2Dot(v1, v2) / (D3DXVec2Length(v1) * D3DXVec2Length(v2)));
		float temp_vx = BOSS_SPEED * cos(angle);
		float temp_vy = BOSS_SPEED * sin(angle);

		if (destinationX - x < 0)
			vx = -temp_vx;
		else
			vx = temp_vx;
		if (destinationY - y < 0)
			vy = -temp_vy;
		else
			vy = temp_vy;
		//DebugOut(L"goc %f do\n", angle * 180 / PI);
		//DebugOut(L"vx %f, vy %f\n", vx, vy);
	}
	else if (isCharging)
	{
		vx = nx*abs(player->x - x) / BOSS_CHARGING_TIME;

	}
}

bool BossBat::IsAtDestination()
{
	if (abs(destinationX - x) <= 5 && abs(destinationY - y) <= 5)
		return true;
	return false;
}
void BossBat::Hover()
{
	isCharging = false;
	isHovering = true;

	PickRandomPositioning();
	SetChargingSpeed();
}
void BossBat::Charge()
{
	isCharging = true;
	isHovering = false;
	nx = (player->x - x)/abs(player->x-x);
	vy = 0;
	if (player->y - y >= 50)
	{
		destinationX = player->x;
		destinationY = player->y;
		CalculateFlyingEquation();
		SetChargingSpeed();
		
	}
	else
	{
		a = DEFAULT_A;
		vx = BOSS_SPEED;
	}
	

}
void BossBat::CalculateFlyingEquation()
{
		a = (  y-destinationY) / ((x -destinationX) * (x - destinationX));
}


