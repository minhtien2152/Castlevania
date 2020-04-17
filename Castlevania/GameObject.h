#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "AnimationSets.h"


using namespace std;



class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};



class CGameObject
{
public:
	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	DWORD dt;

	int state;
	int type;		//0 : static  1: dynamic
	bool isEnabled;
	LPANIMATION_SET animation_set;
	vector<LPCOLLISIONEVENT> staticCoEvents;
	vector<LPCOLLISIONEVENT> dynamicCoEvents;
	int hp;


public:
	
	virtual void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx; this ->vy = vy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void SetDirection(int nx) { this->nx = nx; }
	int GetState() { return this->state; }
	
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	void RenderBoundingBox();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();
	bool IsOverlapping(CGameObject* obj);
	bool IsColiding(CGameObject* obj);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects = NULL, vector<LPGAMEOBJECT>* dynamicCoObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	

	int GetHP() { return hp; }
	void AddHealth(int amount);

	~CGameObject();
};