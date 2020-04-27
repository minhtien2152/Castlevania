#include "Simon.h"
#include "Utils.h"
#include "Define.h"
#include "Zombie.h"
#include "Game.h"
#include "Portal.h"
Simon::Simon()
{
	if (CGame::GetInstance()->GetState() == GAME_STATE_PLAYSCENE)
		mainWeapon = new Whip();
	else mainWeapon = NULL;
	hp = 2;/*SIMON_DEFAULT_HEALTH;*/
	heartsCollected = 5;
	subWeapon = NULL;
}

Simon::~Simon()
{
}

bool Simon::IsAttacking()
{
	return state == SIMON_SIT_ATTACK || state == SIMON_STAND_ATTACK;
}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	if (isWaitingForAni)
		if (animation_set->at(state)->IsOver())
			isWaitingForAni = false;

	if (CGame::GetInstance()->GetState() == GAME_STATE_INTROWALK)
	{
		CGameObject::Update(dt);
		x += dx* 0.5;	//di cham lai
		y += dy;
		return;
	}
	CGameObject::Update(dt, coObjects);
	// simple fall down
	vy += SIMON_GRAVITY*dt;
	if (isTouchingGround)
	{
		isJumping = false;
		if (IsAttacking())
			vx = 0;
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
		vx = 0;
		PullUp();
	
		break;
	case SIMON_WALK:
		PullUp();
	
		vx = -nx*SIMON_WALKING_SPEED;
		
		break;
	case SIMON_JUMP:
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
		vx = 0;

		
		break;
	case SIMON_STAND_ATTACK:
		isSitting = false;
	case SIMON_SIT_ATTACK:	
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
		vx = 0;
		mainWeapon->LevelUp();
		isWaitingForAni = true;
		
		break;
	case SIMON_WAIT:
		vx = 0;
		vy = 0;
		break;
	case SIMON_DEAD:
		vx = 0;
	
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
//	heartsCollected--;
}

Whip* Simon::GetMainWeapon()
{
	return mainWeapon;
}

void Simon::SetHeartsCollected(int heartNum)
{
	this->heartsCollected = heartNum;
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
