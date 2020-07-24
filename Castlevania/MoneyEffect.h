#pragma once
#include "Effect.h"
class MoneyEffect:public Effect
{
	int value;
	int renderNumber,renderTail;
public:
	MoneyEffect(float x, float y,int value);
	virtual void Render();
	virtual bool IsFinished();
	void Reset();
};

