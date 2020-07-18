#include "Ground.h"
#include "Define.h"

Ground::Ground()
{
	isStatic = true;
	isSolid = true;
}

Ground::~Ground()
{
}

void Ground::Render()
{
	if(isDestructable)
		animation_set->at(state)->Render(x, y);
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GROUND_BBOX_WIDTH;
	b = y + GROUND_BBOX_HEIGHT;
}