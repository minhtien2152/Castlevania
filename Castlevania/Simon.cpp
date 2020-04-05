#include "Simon.h"
#include "Utils.h"
#include "Define.h"
#include "Zombie.h"

bool Simon::IsAttacking()
{
	return state == SIMON_SIT_ATTACK || state == SIMON_STAND_ATTACK;
}
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	
	// simple fall down
	vy += SIMON_GRAVITY*dt;

	/*if (isJumping && IsAttacking() && animation_set->at(state)->IsOver())
		CGameObject::SetState(SIMON_JUMP);*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (GetTickCount() - invulnerable_start > SIMON_INVULNERABLE_TIME)
	{
		invulnerable_start = 0;
		isInvulerable = 0;
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.5f;

		if (nx != 0) vx = 0;
		if (ny != 0) {
			vy = 0;
			isJumping = false;
		}

		 //Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CZombie*>(e->obj)) // zombie
			{
				CZombie* zombie = dynamic_cast<CZombie*>(e->obj);
				if (e->ny != 0||e->nx !=0)
				{
					if(!isInvulerable)
						SetState(SIMON_DEFLECT);

				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	/*int tempState = state;
	if (isJumping && IsAttacking() && animations[state]->IsOver())
		tempState = SIMON_JUMP;*/
	//DebugOut(L"[INFO] vy: %d\n", vy);
	int alpha = 255;
	if (isInvulerable > 0)
		alpha /= 2;
	animation_set->at(state)->Render(x, y,nx,alpha);
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
		if (isSitting)
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
	case SIMON_SIT_ATTACK:	case SIMON_STAND_ATTACK:
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_DEFLECT:
		vx = nx*SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		isInvulerable = true;
		invulnerable_start = GetTickCount();
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
	/*if (isSitting)
	{
		left = x + 16;
		top = y+18;
		right = left + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
	else
	{
		left = x + 16;
		top = y+2;
		right = left + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	
	}*/

		left = x + 16;
		top = y + 2;
		right = left + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;

		if (isJumping||isSitting)
			bottom = y + SIMON_BBOX_HEIGHT- SIMON_PULL_UP;
	
}
