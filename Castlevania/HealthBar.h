#pragma once
#include"AnimationSets.h"
#include"GameObject.h"

class HealthBar
{
	float x, y;
	int currHP, defaultHP;
	LPGAMEOBJECT object;
	LPANIMATION_SET animation_set;
	int type;
public:
	HealthBar(float x, float y, LPGAMEOBJECT object,int type);
	~HealthBar();
	void Update();
	void Render();
};

