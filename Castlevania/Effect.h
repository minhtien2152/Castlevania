#pragma once
#include "AnimationSets.h"

class Effect
{
	
protected:
	float x, y;
	LPANIMATION_SET animation_set;
public:
	int isFinished;
	Effect(float x,float y);
	~Effect();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual bool IsFinished();
	void SetAnimationSet(LPANIMATION_SET ani_set);
	virtual void Reset();
};

typedef Effect* LPEFFECT;
