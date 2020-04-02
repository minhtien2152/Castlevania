#pragma once
#include"Define.h"
#include"GameObject.h"


#define SMALL_CANDLE		0
#define BIG_CANDLE			1
#define CANDLE_DESTROYED	2

class Candle:public CGameObject
{
public:
	Candle();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
};

