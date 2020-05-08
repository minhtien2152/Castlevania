#include "Bumper.h"
#include "Define.h"
Bumper::Bumper()
{
	isStatic = true;
	isSolid = false;
	isDestructable = false;
}
void Bumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TILE_WIDTH;
	bottom = top + TILE_HEIGHT;
}

void Bumper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Bumper::Render()
{
	RenderBoundingBox();
}
