#pragma once
#include "AnimationSets.h"

class Effect
{
	
protected:
	float x, y;
	int type;
	LPANIMATION_SET animation_set;
public:
	int isFinished;
	Effect();
	~Effect();
	virtual void Update(DWORD dt);
	virtual void Render();
	bool IsFinished();
	void SetAnimationSet(LPANIMATION_SET ani_set);
	void Reset();
};

typedef Effect* LPEFFECT;
