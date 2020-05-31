#include "FallingBrick.h"
#include "Define.h"
FallingBrick::FallingBrick()
{
	isPhysicEnabled = false;
}

void FallingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += GRAVITY * dt;
}

void FallingBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left=right = x;
	top = bottom = y;
}

void FallingBrick::Render()
{
	animation_set->at(state)->Render(x, y);
}

