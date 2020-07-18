#include "Knight.h"
#include "Utils.h"
#include "Define.h"
#include "Bumper.h"

Knight::Knight()
{
	nx = -1;
	isDestructable = true;
	isDestroyed = false;
	hp = 3;
}

void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	vx = -nx * KNIGHT_WALKING_SPEED;
	vy += GRAVITY * dt;
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if(e->obj->tag == Object_Type::BUMPER)
		{
			isColidingSideways = true;
			
		}

	}
	//DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
	CleanUpCoEvents();
//	DebugOut(L"colliding sideways %d\n", isColidingSideways);
	if (isColidingSideways)
	{
		x +=  -nx * 0.5f;
		nx *= -1;
		isColidingSideways = false;
	}
	

}

void Knight::Render()
{
	animation_set->at(state)->Render(x, y, nx,isUpdatingFrame);
}

void Knight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + KNIGHT_BBOW_WIDTH;
	bottom = top + KNIGHT_BBOW_HEIGHT;
}
