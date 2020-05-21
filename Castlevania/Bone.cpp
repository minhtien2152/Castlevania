#include "Bone.h"
#include "Define.h"
#define BONE_BBOX_WIDTH 32
#define BONE_BBOX_HEIGHT 32
Bone::Bone()
{
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::BONE);
	tag = Weapon_Type::BONE;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isPhysicEnabled = false;
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY * dt;
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BONE_BBOX_WIDTH;
	bottom = top + BONE_BBOX_HEIGHT;
}
