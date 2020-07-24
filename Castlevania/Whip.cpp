#include "Whip.h"
#include "Utils.h"
#include "Define.h"
Whip::Whip()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(Weapon_Type::WHIP));
	tag = Weapon_Type::WHIP;
	CGameObject::SetState(WHIP_LEVEL0);
	damage = 1;
}

Whip::~Whip()
{
}


void Whip::SetPosition(float x, float y)
{
	this->x = x - 90;
	this->y = y;
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (obj->isDestructable)
			if (this->IsColidingAABB(obj) && animation_set->at(state)->IsRenderingLastFrame())
			{
				obj->GetHit(this->damage);
				//DebugOut(L"health %d\n", obj->hp);
			}
	}
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
			left = x +80+ NORMAL_WHIP_BBOX_WIDTH;

		
	}
	else if (nx > 0)
	{

			left = NORMAL_WHIP_BBOX_WIDTH + x;
			if (state == WHIP_LEVEL2)
				left = x + NORMAL_WHIP_BBOX_WIDTH/2;

		

	}

	if (state != WHIP_LEVEL2)
		right = left + NORMAL_WHIP_BBOX_WIDTH;
		
	else 
		right = left + LONG_CHAIN_BBOX_WIDTH;
}

void Whip::LevelUp()
{
	if (state < 2)	state++;
}

