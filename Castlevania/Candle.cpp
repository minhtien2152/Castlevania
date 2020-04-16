#include "Candle.h"

Candle::Candle()
{
	CGameObject::SetState(BIG_CANDLE);
}

void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case BIG_CANDLE:
		right = left + BIG_CANDLE_BBOX_WIDTH;
		bottom = top + BIG_CANDLE_BBOX_HEIGHT;
		break;
	case SMALL_CANDLE:
		right = left + SMALL_CANDLE_BBOX_WIDTH;
		bottom = top + SMALL_CANDLE_BBOX_HEIGHT;
		break;
	default:
		right = left;
		top = bottom;
		break;
	}

}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Candle::Render()
{
	if (state != CANDLE_DESTROYED)
	{
		animation_set->at(state)->Render(x, y);
		RenderBoundingBox();
	}
	
}

