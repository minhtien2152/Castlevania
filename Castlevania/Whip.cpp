#include "Whip.h"
#include "Utils.h"
#include "Zombie.h"
#include "Candle.h"
Whip::Whip()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(Object_Type::WHIP));
	CGameObject::SetState(LEVEL0);
}

Whip::~Whip()
{
}

void Whip::Attack( int nx, bool isSitting)
{
	
	this->nx = nx;
	animation_set->at(state)->Reset();
}



void Whip::SetPosition(float x, float y)
{
	this->x = x- 90;
	this->y = y;
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void Whip::Render()
{
	
	animation_set->at(state)->Render(x,y,nx);

	RenderBoundingBox();
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != LEVEL2)
			left = x + 80 + LONG_CHAIN_BBOX_WIDTH;
		 left = x + 80+ NORMAL_WHIP_BBOX_WIDTH;
	}
	else if (nx > 0)
	{
		if (state != LEVEL2)
			left = LONG_CHAIN_BBOX_WIDTH + x;
		left = NORMAL_WHIP_BBOX_WIDTH + x;
	
	}

	if (state != LEVEL2)
		right = left + NORMAL_WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;
}

void Whip::LevelUp()
{
	if (state < 2)	state++;
}
