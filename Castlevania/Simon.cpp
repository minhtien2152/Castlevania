#include "Simon.h"
#include "Utils.h"
#include "Define.h"
#include "Zombie.h"
#include "Game.h"
#include "Portal.h"
#include "StairBottom.h"
Simon::Simon()
{
	if (CGame::GetInstance()->GetState() == GAME_STATE_PLAYSCENE)
		mainWeapon = new Whip();
	else mainWeapon = NULL;
	hp= SIMON_DEFAULT_HEALTH;
	score = 0;
	life = SIMON_DEFAULT_LIFE;
	heart = SIMON_DEFAULT_HEART;
	subWeapon = NULL;
}

Simon::~Simon()
{
}

bool Simon::IsAttacking()
{
	return state == SIMON_SIT_ATTACK || state == SIMON_STAND_ATTACK || state == SIMON_STAIR_UP_ATK ||state == SIMON_STAIR_DOWN_ATK;
}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	if (CGame::GetInstance()->GetState() == GAME_STATE_INTROWALK)
	{
		CGameObject::Update(dt);
		x += dx * 0.5;	//di cham lai
		y += dy;
		return;
	}
	if (isWaitingForAni)
		if (animation_set->at(state)->IsOver())
			isWaitingForAni = false;
	//DebugOut(L"waiting for ani %d\n", isWaitingForAni);
	if (stairState != 0)
		isTouchingGround = false;
	isCollidingStairObject = false;
	isAllowToGoDown = false;
	isAllowToGoUp = false;


	//DebugOut(L"touchingGround %d\n", isTouchingGround);
	// simple fall down
	if (stairState ==0)
	{
		CGameObject::Update(dt, coObjects);
		vy += SIMON_GRAVITY * dt;
	}
	else
	{
		x += vx * dt;
		y += vy * dt;
	}
	if (isTouchingGround)
	{
		isJumping = false;
		if (IsAttacking())
			vx = onFeetObjVx;
	}

	if (isJumping && IsAttacking() && animation_set->at(state)->IsOver())
	{
		isSitting = false;
		CGameObject::SetState(SIMON_STAND);									//ko co chan lai sau khi danh xong
	}
	
	if(!isInvisible && isInvulnerable)
		if (GetTickCount() - invulnerable_start > SIMON_INVULNERABLE_TIME)
		{
			invulnerable_start = 0;
			isInvulnerable = 0;
		}
	//if(isInvisible)
	//if (GetTickCount() - invisibility_start > SIMON_INVISIBILITY_TIME)
	//{
	//	invisibility_start = 0;
	//	isInvisible = 0;
	//	isInvulnerable = 0;
	//}


	
	

	if (IsAttacking() && !canUseSubWeapon)
	{
		mainWeapon->SetPosition(x, y);
		mainWeapon->SetSpeed(vx, vy);
	}
	else mainWeapon->isEnabled = false;
	
	if (hp == 0)
		SetState(SIMON_DEAD);
	//DebugOut(L"x = %f , y = %f \n", x, y);
	
//	DebugOut(L"NonsolidObject size %d\n", nonSolidObjects.size());
	DebugOut(L"Stairlock %d\n", stairLock);
	DebugOut(L"stairState %d\n", stairState);
	stairObjects.clear();
	for (UINT i = 0; i < nonSolidObjects.size(); i++)
	{
		LPGAMEOBJECT obj = nonSolidObjects[i];

		if (dynamic_cast<StairObject*>(obj))
			stairObjects.push_back(obj);
	}
	if(stairState!=0)
		CheckGetOffStair();
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if (dynamic_cast<CPortal*>(e->obj))
		{
			CPortal* p = dynamic_cast<CPortal*>(e->obj);
			CGame::GetInstance()->SwitchScene(p->GetSceneId());
		}

	}
	
//	DebugOut(L"x= %f,y= %f\n",x, y);
	DebugOut(L"vx= %f,vy= %f\n",vx, vy);
	//DebugOut(L"stairEnterX = %f, isCollidingStairObject %d,currentStairType %d\n", stairEnterX, isCollidingStairObject,currentStairType);
	CleanUpCoEvents();
}

void Simon::Render()
{
	
	int alpha = 255;
	if (isInvulnerable  || isInvisible)
		alpha /= 2;

		animation_set->at(state)->Render(x, y, nx, 1, alpha);
	if (IsAttacking())
	{
		if (!canUseSubWeapon)
			mainWeapon->Render();
	}
	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_STAND:
		//DebugOut(L"onFeetObjVx %f", onFeetObjVx);
		vx = onFeetObjVx;
		PullUp();
		stairState = STAIR_STATE_NONE;
		break;
	case SIMON_WALK:
		PullUp();
	
		vx = -nx*SIMON_WALKING_SPEED;
		
		break;
	case SIMON_JUMP:
		if (isColidingSideways)
			vx = 0;
		vy = -SIMON_JUMP_SPEED_Y;
		isJumping = true;
		isSitting = true;
		break;
	case SIMON_SIT:
		if (isSitting == false)
		{
			y = y + SIMON_PULL_UP;
		}
		isSitting = true;
		vx = onFeetObjVx;

		
		break;
	case SIMON_STAND_ATTACK:
		isSitting = false;
	case SIMON_SIT_ATTACK:	
	case SIMON_STAIR_UP_ATK:
	case SIMON_STAIR_DOWN_ATK:
		isWaitingForAni = true;
		
		ResetAni();
		break;
	case SIMON_DAMAGED:
		vx = nx*SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;
		isInvulnerable = true;
		isJumping = true;
		invulnerable_start = GetTickCount();
		isWaitingForAni = true;
		ResetAni();
		break;
	case SIMON_POWERUP:
		ResetAni();
		isWaitingForAni = true;
		vx = 0;
		mainWeapon->LevelUp();
		
		
		break;
	case SIMON_WAIT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_DEAD:
		vx = 0;
	
		break;
	case SIMON_STAIR_UP:
		ResetAni();
		isWaitingForAni = true;
		vx = -nx*SIMON_STAIR_SPEED;
		vy = -SIMON_STAIR_SPEED;
		stairState = STAIR_STATE_GOING_UP;
		break;
	
		break;
	case SIMON_STAIR_DOWN:
		ResetAni();
		isWaitingForAni = true;
		vx = -nx * SIMON_STAIR_SPEED;
		vy = SIMON_STAIR_SPEED;
		stairState = STAIR_STATE_GOING_DOWN;
		break;
	case SIMON_STAIR_UP_IDLE:
	case SIMON_STAIR_DOWN_IDLE:
		vx = vy = 0;
		break;

	default:
		break;
	}
}

void Simon::PullUp()
{
	if (isSitting)
	{
		isSitting = false;
		y = y - SIMON_PULL_UP;

	}
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

		left = x +16;
		top = y;
		right = left + SIMON_BBOX_WIDTH;
		bottom = top + SIMON_BBOX_HEIGHT;

		if (isSitting)
			bottom = y + SIMON_BBOX_HEIGHT- SIMON_PULL_UP;
	
}

void Simon::AttackWithWhip()
{
	canUseSubWeapon = false;
	mainWeapon->Attack(this);
}

void Simon::AttackWithSubWeapon()
{

	subWeapon->Attack(this);
	//heart--;
}

Whip* Simon::GetMainWeapon()
{
	return mainWeapon;
}

void Simon::SetScore(int score)
{
	this->score = score;
}

void Simon::SetHeartsCollected(int heartNum)
{
	this->heart = heartNum;
}

void Simon::SetLife(int life)
{
	this->life = life;
}

void Simon::StartInvisibilityTimer()
{
	invisibility_start = GetTickCount();
	isInvisible = true;
	isInvulnerable = true;
}

void Simon::SetSubWeapon(CWeapon* wp)
{
	subWeapon = wp;
}




CWeapon* Simon::GetSubWeapon()
{
	return subWeapon;
}

int Simon::GetCurrentStairDirection()
{
	return currentStairDirection;
}

int Simon::GetCurrentStairType()
{
	return currentStairType;
}

float Simon::GetStairEnterPosX()
{
	return stairEnterX;
}

void Simon::GoToStairEnterPos()
{
	if (x - stairEnterX > 0)
		SetDirection(1);
	else
		SetDirection(-1);
	SetState(SIMON_WALK);
}

void Simon::GoIntoStair(int upOrDown, int direction)
{
	SetDirection(direction);
	if (upOrDown == 1)
		SetState(SIMON_STAIR_DOWN);
	else 
		SetState(SIMON_STAIR_UP);
}

void Simon::ProcessStair(int type)
{
	for (UINT i = 0; i < stairObjects.size(); i++)
	{
		StairObject* obj = dynamic_cast<StairObject*>(stairObjects[i]);
		if (this->IsColidingAABB(obj))
		{


			if(stairState ==0)
				if (obj->GetType()  == type)
				{
					isCollidingStairObject = true;
					currentStairDirection = obj->nx;
					currentStairType = ((StairObject*)obj)->GetType();

					stairEnterX = ((StairObject*)obj)->GetEnterPosX();
					if (obj->IsInRightPosToEnterStair(x, y))
					{
						if (currentStairType == -1)
							isAllowToGoUp = true;

						else if (currentStairType == 1)

							isAllowToGoDown = true;
					}
				}
				
			



		}


		
	}
}

void Simon::CheckGetOffStair()
{
	for (UINT i = 0; i < stairObjects.size(); i++)
	{
		StairObject* obj = dynamic_cast<StairObject*>(stairObjects[i]);
		if (this->IsColidingAABB(obj))
		{
			if (stairState != 0)
			{
				if (((StairObject*)obj)->nx == currentStairDirection && ((StairObject*)obj)->GetType() == currentStairType || ((StairObject*)obj)->nx == -currentStairDirection && ((StairObject*)obj)->GetType() == -currentStairType)
					SetState(SIMON_STAND);
			}
		}
	}
}
