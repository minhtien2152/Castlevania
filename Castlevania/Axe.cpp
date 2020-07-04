#include "Axe.h"
#include "Define.h"

Axe::Axe()
{
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::AXE);
	tag = Weapon_Type::AXE;
	cost = 1;
}
Axe::~Axe()
{
}
void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY * dt;
	if (isTouchingGround)
	{
		isEnabled = false;
		return;
	}
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if (e->obj->isDestructable)
		{
			e->obj->GetHit(this->damage);
		}
	}

	//DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
}
void Axe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + AXE_BBOX_WIDTH;
	bottom = top + AXE_BBOX_HEIGHT;
}
void Axe::Attack(LPGAMEOBJECT user)
{
	CWeapon::Attack(user);
	vx = -nx * AXE_SPEED_X;
	vy = -AXE_SPEED_Y;
}
