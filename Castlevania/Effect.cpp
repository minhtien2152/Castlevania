#include "Effect.h"



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
	animation_set->at(0)->Render(x,y);
}

bool Effect::IsFinished()
{
	return animation_set->at(0)->IsOver();
}


void Effect::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
}

void Effect::Reset()
{
	animation_set->at(0)->SetAniStartTime(GetTickCount());
	animation_set->at(0)->Reset();
}