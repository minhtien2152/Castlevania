#include "Item.h"
#include"Define.h"

Item::Item()
{
	appearTime = GetTickCount();
	animation_set = CAnimationSets::GetInstance()->Get(ITEM_ANI_SET_ID);
	
}

void Item::Render()
{
	
		animation_set->at(state)->Render(x, y);
		RenderBoundingBox();
}

void Item::	Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY * dt;
	if (GetTickCount() - appearTime > ITEM_APPEAR_TIME)
		this->isEnabled = false;
	
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
		bottom = top + 25;
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
		bottom = top + 32;
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
	case CHICKEN:
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

LPANIMATION Item::GetCurrentAni()
{
	return animation_set->at(state);
}