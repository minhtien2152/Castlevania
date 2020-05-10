#include "MovingPlatform.h"
#define PLATFORM_SPEED			0.1
#define PLATFORM_BBOW_WIDTH		64
#define PLATFORM_BBOW_HEIGHT	16
MovingPlatform::MovingPlatform()
{
	isDestructable = false;
	isStatic = false;
	isSolid = true;
}
void MovingPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + PLATFORM_BBOW_WIDTH;
	bottom = top + PLATFORM_BBOW_HEIGHT;
}

void MovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vx = -nx * PLATFORM_SPEED;
	vy = 0;
	if (isColidingSideways)
	{
		x += -nx * 0.5f;
		nx *= -1;
	}

	CleanUpCoEvents();
	//	DebugOut(L"colliding sideways %d\n", isColidingSideways);

}

void MovingPlatform::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}
