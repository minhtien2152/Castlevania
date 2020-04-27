#include "Dagger.h"
#include "Define.h"
Dagger::Dagger()
{
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::DAGGER);

}
Dagger::~Dagger()
{
}
void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isColidingSideways)		//collision with solid object only
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
	CleanUpCoEvents();
//	DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
}
void Dagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

void Dagger::Attack(LPGAMEOBJECT user)
{
	CWeapon::Attack(user);
	vx = -nx * DAGGER_SPEED;
	vy = 0;
}

void Dagger::Render()
{
	RenderBoundingBox();
	animation_set->at(state)->Render(x, y, -nx);
}
