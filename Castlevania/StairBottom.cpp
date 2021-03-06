#include "StairBottom.h"
#include "Define.h"


StairObject::StairObject(int dir, int type,int id)
{
	nx = dir;
	this->id = id;
	this->type = type;
	isStatic = true;
	isSolid = false;
	isDestructable = false;
}

void StairObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = left + TILE_WIDTH*1.5f;
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
	/*if(nx ==-1)*/
	//if (type == -1)
	//	return x + 16;
	//return x - TILE_WIDTH;
	if(nx == -1)
	return x;
	return x - TILE_WIDTH;
}

bool StairObject::IsInRightPosToEnterStair(float simonX, float simonY)
{
	if (simonX >= GetEnterPosX() - 2 && simonX <= GetEnterPosX() + 2)
		if (type == 1)
		{
			if (simonY >= y - 2 && simonY <= y + 2)
				return true;
		}
		else
		{
			if (simonY +32 >= y - 2 && simonY  +32 <= y + 2)
				return true;
		}
	return false;
}

int StairObject::GetId()
{
	return id;
}

int StairObject::GetType()
{
	return type;
}
