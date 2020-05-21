#include "Skeleton.h"
#include "Define.h"
#define SKELETON_MOVING	0
#define SKELETON_JUMP	1
#define SKELETON_ATTACK 2
#define SKELETON_SPEED_VX 0.1
#define SKELETON_JUMP_SPEED_VX 0.35
#define SKELETON_SPEED_VY	0.4
#define SKELETON_ATTACK_INTERVAL 600
#define SKELETON_BBOX_WIDTH		32
#define SKELETON_BBOX_HEIGHT	63

Skeleton::Skeleton() :SmartEnemy()
{
	lastAttack = 0;
	timeAccumulated = -1;
	nx_backUp = nx;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SmartEnemy::Update(dt, coObjects);
	if (player == NULL)
	{
		x_backUp = x;
		
	}
	vy += GRAVITY * dt;
	AdjustDirectionToFollowSimon();
	if (player != NULL)
	{

			if (isTouchingGround)
			{
				if (isColidingSideways)
					nx_backUp *= -1;
				int onEdge = IsOnEdge();
				if (onEdge!=0 && onEdge == nx_backUp)
					if (JumpingSimulator())	
						SetState(SKELETON_JUMP);
				if (nx == 1)
				{
					if (x >= x_backUp + SIMON_BBOX_WIDTH + TILE_WIDTH * 2  )
						nx_backUp = 1;
					else if (x <= x_backUp + SIMON_BBOX_WIDTH - TILE_WIDTH * 2)
						nx_backUp = -1;
				}
				else if(nx == -1)
				{
					if (x >= x_backUp  + TILE_WIDTH * 2)
						nx_backUp = 1;
					else if (x <= x_backUp  - TILE_WIDTH * 2)
						nx_backUp = -1;
				}
				SetState(SKELETON_MOVING);
				if (GetTickCount() - lastAttack >= SKELETON_ATTACK_INTERVAL)
					SetState(SKELETON_ATTACK);

			}
		if (state == SKELETON_ATTACK && animation_set->at(state)->IsOver())
			SetState(SKELETON_MOVING);
		

	}
	if (timeAccumulated >= 1000 || timeAccumulated == -1)
	{
		timeAccumulated = 0;
		AdjustMovementCenter();
	}
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	case SKELETON_MOVING:
		vx = -nx_backUp * SKELETON_SPEED_VX;
		break;
	case SKELETON_JUMP:
		vy = -SKELETON_SPEED_VY;
		vx = -nx_backUp * SKELETON_JUMP_SPEED_VX;
		break;
	case SKELETON_ATTACK:
		lastAttack = GetTickCount();
		vx = 0;
		ResetAni();

		ProcessWeapon();
		break;
	default:
		break;
	}
}

void Skeleton::AdjustMovementCenter()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		if (plr_x - x > 0)
			x_backUp = plr_x - TILE_WIDTH * 3;
		else
			x_backUp = plr_x + SIMON_BBOX_WIDTH + TILE_WIDTH * 3;
	}
}

void Skeleton::SetProjectTileList(vector<CWeapon*>* projectTileList)
{
	this->projectTileList = projectTileList;
}

void Skeleton::ProcessWeapon()
{
	
		LPWEAPON wp = new Bone();
		wp->Attack(this);
		projectTileList->push_back(wp);
		lastAttack = GetTickCount();
		//DebugOut(L"created bone\n");
	
}

bool Skeleton::CheckJumpingCondition()
{
	float plr_x, plr_y;
	player->GetPosition(plr_x, plr_y);
	if (abs(plr_x - x) < TILE_WIDTH)
		return true;
	return false;
}

bool Skeleton::JumpingSimulator()
{
	LPGAMEOBJECT decoy = CreateDecoy(x,y);
	decoy->SetState(SKELETON_JUMP);
	float dc_x, dc_y;
	while (decoy->GetPosY()<screen_height)
	{
		
		decoy->Update(dt, &solidObjects);
		dc_x = decoy->GetPosX();
		dc_y = decoy->GetPosY();
		if (decoy->isTouchingGround)
		{
			free(decoy);
			decoy = NULL;
			if (dc_y - y <= 0 && abs(dc_x -x)>=TILE_WIDTH)
				return true;
			else
				return false;

		}
	}
	free(decoy);
	decoy = NULL;
	return false;
}

int Skeleton::IsOnEdge()
{
	SmartEnemy* decoyleft = CreateDecoy(x- SKELETON_BBOX_WIDTH/2 ,y);
	SmartEnemy* decoyright = CreateDecoy(x + SKELETON_BBOX_WIDTH/2, y);
	decoyleft->SetSpeed(0, GRAVITY * dt);
	decoyright->SetSpeed(0, GRAVITY * dt);

	int i = 2;
	while (i--) {
		decoyleft->Update(dt, &solidObjects);
		decoyright->Update(dt, &solidObjects);
	}
	float dc_left_y = decoyleft->GetPosY();
	float dc_right_y = decoyright->GetPosY();
	free(decoyleft);
	free(decoyright);
	decoyleft = decoyright = NULL;
	if (dc_left_y > y)
		return 1;
	else if (dc_right_y > y)
		return -1;
	return 0;

}

SmartEnemy* Skeleton::CreateDecoy(float x, float y)
{
	SmartEnemy* decoy = new Skeleton();
	decoy->SetPosition(x, y);
	decoy->SetDirection(nx);
	return decoy;
}
