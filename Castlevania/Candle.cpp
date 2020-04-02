#include "Candle.h"

Candle::Candle()
{
}

void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Candle::Render()
{
	animation_set->at(state)->Render(x, y);
}

void Candle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BIG_CANDLE:
		break;
	default:
		break;
	}
}
