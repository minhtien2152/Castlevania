#include "GameObject.h"
#include <algorithm>

#include "Utils.h"
#include "Game.h"
#include "Textures.h"
#include "FireEffect.h"
#include "SparkEffect.h"
CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
	isEnabled = true;
	state = 0;
	hp = 1;
	isPhysicEnabled = true;
	isUpdatingFrame = true;
}

bool CGameObject::IsColidingAABB(CGameObject* obj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);
	obj->GetBoundingBox(l1, t1, r1, b1);

	if (CGame::GetInstance()->IsOverlapping(l, t, r, b, l1, t1, r1, b1))
		return true;

	return false;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	if (hp <= 0)
		isDestroyed = true;
	if (isStatic || isDestroyed)	return;
	
	dx = vx * dt;
	dy = vy * dt;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	coEventsResult.clear();
	nonSolidObjects.clear();
	vector<LPGAMEOBJECT> solidObjects;
	
	if (coObjects != NULL)
	for (int i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->isSolid)
			solidObjects.push_back(coObjects->at(i));
		else nonSolidObjects.push_back(coObjects->at(i));
	if (isPhysicEnabled == false)
		return;
	CalcPotentialCollisions(&solidObjects, coEvents);
	CalcPotentialCollisions(&nonSolidObjects, nonSolidObjCoEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		isColidingSideways = false;
		isTouchingGround = false;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		/*if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);*/
		x += min_tx * dx + nx * 0.5f;
		
		

		if (nx != 0) {
			vx = 0;
			isColidingSideways = true;
		}
		else
			isColidingSideways = false;
		if (ny < 0) {

			vy = 0;
			y += min_ty * dy + ny * 0.5f;
			isTouchingGround = true;
			

		}
		else
		{
			isTouchingGround = false;
			y += dy;
		}
		//DebugOut(L"coeventResult %d\n", coEventsResult.size());
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[0];
		
				onFeetObjVx = e->obj->GetVx();


		}
			
			

		
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGameObject::CleanUpCoEvents()
{
	for (UINT i = 0; i < nonSolidObjCoEvents.size(); i++)
		delete nonSolidObjCoEvents[i];
	nonSolidObjCoEvents.clear();
	
}

int CGameObject::GetPosX()
{
	return x;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}



void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, nx,bbox, 0, 0, rect.right, rect.bottom, 32);
}










void CGameObject::SetHP(int hp)
{
	this->hp = hp;
}

void CGameObject::AddHealth(int amount)
{
	this->hp += amount;
}

void CGameObject::ResetAni()
{
	animation_set->at(state)->Reset();
	animation_set->at(state)->SetAniStartTime(GetTickCount());
}

CGameObject::~CGameObject()
{
}