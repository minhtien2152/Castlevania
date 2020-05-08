#include "StairBottom.h"
#include "Define.h"


StairObject::StairObject(int dir, int type)
{
	nx = dir;
	this->type = type;
	isStatic = true;
	isSolid = false;
	isDestructable = false;
}

void StairObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = left + TILE_WIDTH;
	top = y + 31;
	if (type == 1)
		top = y;
	bottom = top + 1;
}

void StairObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void StairObject::Render()
{
	RenderBoundingBox();
}

float StairObject::GetEnterPosX()
{
	return x - 32;
}

bool StairObject::IsInRightPosToEnterStair(float simonX)
{
	if (simonX >= x - TILE_WIDTH -2 && simonX <= x - TILE_WIDTH +2)
		return true;
	return false;
}

int StairObject::GetType()
{
	return type;
}
