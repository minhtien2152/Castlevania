#include "MoneyEffect.h"
#include "Define.h"

#define MONEY_ONE	0
#define MONEY_TWO	1
#define MONEY_FOUR	2
#define MONEY_SEVEN	3
#define MONEY_HUNDRED	4
#define MONEY_THOUSAND	5
#define MONEY_NUMBER_WIDTH	16
MoneyEffect::MoneyEffect(float x, float y,int value): Effect(x,y)
{
	this->value = value;
	animation_set = CAnimationSets::GetInstance()->Get(Effect_Type::MONEY_EFFECT);
	int renderValue;
	if (value / 1000 > 0)
	{
		renderValue = value / 1000;
		renderTail = MONEY_THOUSAND;
	}
	else
	{
		renderValue = value / 100;
		renderTail = MONEY_HUNDRED;
	}

	switch (renderValue)
	{
	case 1:
		renderNumber = MONEY_ONE;
		break;
	case 2:
		renderNumber = MONEY_TWO;
		break;
	case 4:
		renderNumber = MONEY_FOUR;
		break;
	case 7:
		renderNumber = MONEY_SEVEN;
		break;
	}
	Reset();
}

void MoneyEffect::Render()
{
	animation_set->at(renderNumber)->Render(x, y);
	animation_set->at(renderTail)->Render(x+ MONEY_NUMBER_WIDTH, y);
}

bool MoneyEffect::IsFinished()
{
	return animation_set->at(renderNumber)->IsOver();
}

void MoneyEffect::Reset()
{
		animation_set->at(renderNumber)->SetAniStartTime(GetTickCount());
		animation_set->at(renderNumber)->Reset();
}
