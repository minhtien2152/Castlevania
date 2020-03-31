#include "AnimationFrame.h"

CAnimationFrame::CAnimationFrame(LPSPRITE sprite, int time)
{
	this->sprite = sprite; 
	this->time = time;
}

DWORD CAnimationFrame::GetTime()
{
	return this->time;
}

LPSPRITE CAnimationFrame::GetSprite()
{
	return this->sprite;
}
