#pragma once
#include "Effect.h"
#include "FallingBrick.h"
class BrickEffect :public Effect
{
	vector<LPGAMEOBJECT> falling_bricks;
	
public:
	float screen_height;
	BrickEffect(float x, float y);
	void InitBricks();
	virtual void Update(DWORD dt);
	virtual bool IsFinished();
	virtual void Render();


};

