#include "FireEffect.h"
#include "Define.h"
#include "AnimationSets.h"
FireEffect::FireEffect(float x, float y):Effect()
{
	this->x = x;
	this->y = y;
	animation_set = CAnimationSets::GetInstance()->Get(Effect_Type::FIRE_EFFECT);
	Reset();
}

FireEffect::~FireEffect()
{
}
