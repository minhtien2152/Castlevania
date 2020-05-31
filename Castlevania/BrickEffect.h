#pragma once
#include "Effect.h"
#include "FallingBrick.h"
class BrickEffect :public Effect
{
	vector<LPGAMEOBJECT> falling_bricks;
public:
	BrickEffect(float x, float y);
	void InitBricks();
	virtual void Update(DWORD dt);


};

