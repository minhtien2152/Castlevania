#include "Item.h"
#include"Define.h"

Item::Item()
{
	appearTime = -1;
	animation_set = CAnimationSets::GetInstance()->Get(7);
}

void Item::Render()
{
	//if (GetTickCount() - appearTime > ITEM_APPEAR_TIME)
		animation_set->at(state)->Render(x, y);
		RenderBoundingBox();
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	// Check collision between zombie and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


		CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		vy += GRAVITY * dt;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}
		else if (ny == -1.0f)
		{
			vy = 0;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case STOPWATCH_ITEM:
		right = left + 30;
		bottom = top + 32;
		break;
	case DAGGER_ITEM:
		right = left + 32;
		bottom = top + 18;
		break;
	case AXE_ITEM:
		right = left + 30;
		bottom = top + 28;
		break;
	case HOLYWATER_ITEM:
		right = left + 32;
		bottom = top + 28;
		break;
	case BOOMERANG_ITEM:
		right = left + 30;
		bottom = top + 28;
		break;
	case SMALLHEART:
		right = left + 16;
		bottom = top + 16;
		break;
	case LARGEHEART:
		right = left + 24;
		bottom = top + 20;
		break;
	case CROSS:
		right = left + 32;
		bottom = top + 32;
		break;
	case INVISPOTION:
		right = left + 29;
		bottom = top + 36;
		break;
	case CHAIN:
		right = left + 32;
		bottom = top + 32;
		break;
	case REDMONEYBAG:
	case BLUEMONEYBAG:
	case WHITEMONEYBAG:
		right = left + 30;
		bottom = top + 30;
		break;
	case DOUBLESHOT:
	case TRIPLESHOT:
		right = left + 28;
		bottom = top + 28;
		break;
	case PORKCHOP:
		right = left + 32;
		bottom = top + 26;
		break;
	case MAGICCRYSTAL:
		right = left + 26;
		bottom = top + 32;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void Item::SetState(int state)
{
	CGameObject::SetState(state);
}

LPSPRITE Item::GetCurrentSprite()
{
	return animation_set->at(state)->GetLPFirstFrame()->GetSprite();
}
