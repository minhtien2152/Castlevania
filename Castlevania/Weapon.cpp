#include "Weapon.h"
#include "Utils.h"
CWeapon::CWeapon()
{
	damage = 1;
	isEnabled = false;
	isStatic = false;
}

CWeapon::~CWeapon()
{
}

void CWeapon::Attack(LPGAMEOBJECT user)
{
	float obj_x, obj_y;
	user->GetPosition(obj_x, obj_y);
	SetPosition(obj_x, obj_y);
	isEnabled = true;
	nx = user->nx;
	ResetAni();
}

void CWeapon::SetPosition(float x, float y)
{
	CGameObject::SetPosition(x, y);
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isTouchingGround || isColidingSideways)
	{
		isEnabled = false;
		return;
	}
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = nonSolidObjCoEvents[i];

		if (e->obj->isDestructable)
		{
			e->obj->AddHealth(-this->damage);
			this->isEnabled = false;
			break;
		}

	}
	DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
	
}

void CWeapon::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	RenderBoundingBox();
}
