#include "FireEffect.h"
#include "Define.h"
FireEffect::FireEffect(float x, float y):Effect()
{
	this->x = x;
	this->y = y;
	type = Effect_Type::FIRE_EFFECT;
	Reset();
}

FireEffect::~FireEffect()
{
}
