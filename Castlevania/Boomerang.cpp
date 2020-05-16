#include "Boomerang.h"
#include "Define.h"
#include "Simon.h"
Boomerang::Boomerang()
{
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::BOOMERANG);
	tag = Weapon_Type::BOOMERANG;
	cost = 1;
}
Boomerang::~Boomerang()
{
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vx += nx * BOOMERANG_TURNBACK_SPEED;
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if (e->obj->isDestructable)
		{
			e->obj->AddHealth(-this->damage);	
		}
		else if (dynamic_cast<Simon*>(e->obj))
		{
			this->isEnabled = false;
			break;
		}

	}
	//DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
	CleanUpCoEvents();
	
}
void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOOMERANG_BBOX_WIDTH;
	bottom = top + BOOMERANG_BBOX_HEIGHT;
}
void Boomerang::Attack(LPGAMEOBJECT user)
{
	CWeapon::Attack(user);
	vx = -nx * BOOMERANG_SPEED;
	vy = 0;
}
