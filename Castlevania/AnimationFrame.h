#pragma once
#include"Sprite.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time);
	DWORD GetTime();
	LPSPRITE GetSprite();
};

typedef CAnimationFrame* LPANIMATION_FRAME;