#include "Effect.h"

LPANIMATION_SET Effect::animation_set = NULL;

Effect::Effect()
{
	
	isFinished = false;
}

Effect::~Effect()
{
}

void Effect::Update(DWORD dt)
{
	
	isFinished = IsFinished();
}

void Effect::Render()
{
	animation_set->at(type)->Render(x,y);
}

bool Effect::IsFinished()
{
	return animation_set->at(type)->IsOver();
}


void Effect::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
}

void Effect::Reset()
{
	animation_set->at(type)->SetAniStartTime(GetTickCount());
	animation_set->at(type)->Reset();
}