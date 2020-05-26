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
	stateWaitingToBeRendered = -1;
	hp = 16;
	subWeaponType = -1;
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
		isPhysicEnabled = false;
		CGameObject::Update(dt);
		x += dx * 0.5;	//di cham lai

		y += dy;
		return;
	}

	if (isWaitingForAni)
		if (animation_set->at(state)->IsOver())
		{
			isWaitingForAni = false;
			if (stateWaitingToBeRendered != -1)
				SetState(stateWaitingToBeRendered);
			stateWaitingToBeRendered = -1;
		}
	//DebugOut(L"waiting for ani %d\n", isWaitingForAni);


	//DebugOut(L"touchingGround %d\n", isTouchingGround);
	// simple fall down
	if (stairState ==0)
	{
		isPhysicEnabled = true;
		vy += SIMON_GRAVITY * dt;
	}
	else {
		isPhysicEnabled = false;
	}
	CGameObject::Update(dt, coObjects);
	if (stairState != 0)
		isTouchingGround = false;
	isCollidingStairObject = false;
	isAllowToGoDown = false;
	isAllowToGoUp = false;

	if (isTouchingGround)
	{
		isJumping = false;
		if (IsAttacking())
			vx = onFeetObjVx;
	}

	if (isJumping && IsAttacking() && animation_set->at(state)->IsOver())
		CGameObject::SetState(SIMON_STAND);									//ko co chan lai sau khi danh xong
	
	
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


	
	

	if (IsAttacking() && mainWeapon->isEnabled)
	{
		mainWeapon->SetPosition(x, y);
		mainWeapon->SetSpeed(vx, vy);
		if (animation_set->at(state)->IsOver())
			mainWeapon->isEnabled = false;
	}
	
	if (hp <= 0)
		SetState(SIMON_DEAD);
	//DebugOut(L"x = %f , y = %f \n", x, y);
	
//	DebugOut(L"NonsolidObject size %d\n", nonSolidObjects.size());
	//DebugOut(L"stairState %d\n", stairState);
	stairObjects.clear();
	for (UINT i = 0; i < nonSolidObjects.size(); i++)
	{
		LPGAMEOBJECT obj = nonSolidObjects[i];

		if (dynamic_cast<StairObject*>(obj))
			stairObjects.push_back(obj);
	}
	//DebugOut(L"stairsize %d\n", stairObjects.size());
	if(stairState!=0)
		CheckGetOffStair();
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if (dynamic_cast<CPortal*>(e->obj))
		{
			CPortal* p = dynamic_cast<CPortal*>(e->obj);
			CGame::GetInstance()->SwitchScene(p->GetSceneId());
			return;
		}

	}
	DebugOut(L"State %d\n", state);
//	DebugOut(L"x= %f,y= %f\n",x, y);
	//DebugOut(L"vx= %f,vy= %f\n",vx, vy);
	//DebugOut(L"stairEnterX = %f, isCollidingStairObject %d,currentStairType %d\n", stairEnterX, isCollidingStairObject,currentStairType);
}

void Simon::Render()
{
	
	int alpha = 255;
	if (isInvulnerable  || isInvisible)
		alpha /= 2;

		animation_set->at(state)->Render(x, y, nx, 1, 1, alpha);
	if (IsAttacking())
	{
		if (mainWeapon->isEnabled)
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
		isWaitingForAni = true;
		ResetAni();
		break;
	case SIMON_STAIR_UP_ATK:
	
	case SIMON_STAIR_DOWN_ATK:
		vx = 0;
		vy = 0;
		isWaitingForAni = true;
		ResetAni();
		break;
	case SIMON_DAMAGED:
		vx = nx * SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;
		isJumping = true;
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
		stairState = 0;
		break;
	case SIMON_STAIR_UP:
		ResetAni();
		isSitting = false;
		isWaitingForAni = true;
		vx = -nx*SIMON_STAIR_SPEED;
		vy = -SIMON_STAIR_SPEED;
		stairState = STAIR_STATE_GOING_UP;
		break;
	
	case SIMON_STAIR_DOWN:
		ResetAni();
		isSitting = false;
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

	mainWeapon->Attack(this);
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

void Simon::SetSubWeaponItem(int type)
{
	subWeaponType = type;
}




int Simon::GetSubWeaponType()
{
	return subWeaponType;
}

void Simon::SetSupWeaponCap(int capacity)
{
	subWeaponMaxCap = capacity;
}

int Simon::GetSubWeaponCap()
{
	return subWeaponMaxCap;
}

bool Simon::IsInRightFrameToUseSubWeapon()
{
	if (IsAttacking() && animation_set->at(state)->IsRenderingLastFrame())
		return true;
	return false;
}

int Simon::GetCurrentStairDirection()
{
	return currentStairDirection;
}

int Simon::GetCurrentStairType()
{
	return currentStairType;
}

void Simon::SetCurrentStairDirection(int dir)
{
	currentStairDirection = dir;
}

void Simon::SetCurrentStairType(int type)
{
	currentStairType = type;
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
	if (stairState == 0)
	for (UINT i = 0; i < stairObjects.size(); i++)
	{
		StairObject* obj = dynamic_cast<StairObject*>(stairObjects[i]);
		if (this->IsColidingAABB(obj))
		{
			if (obj->GetType() == type)
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
				if (((StairObject*)obj)->GetType() == -stairState && obj->nx == -nx)
				{
					SetState(SIMON_STAND);
					if (((StairObject*)obj)->GetType() == STAIR_STATE_GOING_UP)
						y -= 1;
				}
			}
		}
	}
}

void Simon::TakeDamage(int damage)
{
	isInvulnerable = true;
	invulnerable_start = GetTickCount();
	AddHealth(-damage);
}

void Simon::SetStateToBeRender(int state)
{
	stateWaitingToBeRendered = state;
}
