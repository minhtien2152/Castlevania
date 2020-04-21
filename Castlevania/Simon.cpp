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
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects , vector<LPGAMEOBJECT>* dynamicCoObjects)
{
	

	if (CGame::GetInstance()->GetState() == GAME_STATE_INTROWALK)
	{
		CGameObject::Update(dt);
		x += dx* 0.5;	//di cham lai
		y += dy;
		return;
	}
	CGameObject::Update(dt, staticCoObjects, dynamicCoObjects);
	// simple fall down
	vy += SIMON_GRAVITY*dt;

	if (isJumping && IsAttacking() && animation_set->at(state)->IsOver())
		CGameObject::SetState(SIMON_JUMP);									//co chan lai sau khi danh xong

	
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


	vector<LPCOLLISIONEVENT> staticCoEventsResult;

	// No collision occured, proceed normally
	if (staticCoEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(staticCoEvents, staticCoEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		/*if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);*/

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = 0;
			isJumping = false;
			if (IsAttacking())
				vx = 0;
		}

		for (UINT i = 0; i < staticCoEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = staticCoEventsResult[i];

			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				break;
			}
		}
	}
	

	if (IsAttacking())
	{

		
		if (!canUseSubWeapon)
		{
			mainWeapon->SetPosition(x, y);
			mainWeapon->SetSpeed(vx, vy);
		}
	
	}

	if (hp == 0)
		SetState(SIMON_DEAD);
	//DebugOut(L"can use sub %d\n", canUseSubWeapon);
	// clean up collision events
	for (UINT i = 0; i < staticCoEvents.size(); i++) delete staticCoEvents[i];
}

void Simon::Render()
{
	
	int alpha = 255;
	if (isInvulnerable == true)
		alpha /= 2;

	if (isInvisible)
		animation_set->at(state + 14)->Render(x, y, nx);
	else
		animation_set->at(state)->Render(x, y, nx, 1, alpha);
	if (IsAttacking())
	{
		if (!canUseSubWeapon)
			mainWeapon->Render();
		/*else if (isUsingSubWeapon && animation_set->at(state)->IsOver() == true)
			isUsingSubWeapon = false;*/
	}
	//animations[state]->SetFrame(animations[tempState]->GetCurrentFrame());
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
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_DAMAGED:
		vx = nx*SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isInvulnerable = true;
		isJumping = true;
		invulnerable_start = GetTickCount();
		break;
	case SIMON_POWERUP:
		vx = 0;
		mainWeapon->LevelUp();
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
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

		left = x + 16;
		top = y + 2;
		right = left + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;

		if (isJumping&&IsAttacking()&&animation_set->at(state)->IsOver()||isSitting)
			bottom = y + SIMON_BBOX_HEIGHT- SIMON_PULL_UP;
	
}

void Simon::AttackWithWhip()
{
	canUseSubWeapon = false;
	mainWeapon->Attack(nx, isSitting);
}

void Simon::AttackWithSubWeapon()
{
	/*if (animation_set->at(state)->IsRenderingLastFrame())
	{*/
		subWeapon->SetPosition(x, y);
		subWeapon->SetDirection(this->nx);
		subWeapon->isEnabled = true;
		//isUsingSubWeapon = true;
		subWeapon->Attack();
		heartsCollected--;
		//DebugOut(L" simon = %d , weapon = %d ", nx, subWeapon->nx);
	/*}
	DebugOut(L"state %d\n", state);*/
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



void Simon::SetSubWeapon(Weapon_Type subwp)
{
	if (subWeapon == NULL)	subWeapon = new SubWeapon();
	subWeapon->SetState(subwp);
}

SubWeapon* Simon::GetSubWeapon()
{
	return subWeapon;
}
