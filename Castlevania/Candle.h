#pragma once
#include"Define.h"
#include"GameObject.h"


class Candle:public CGameObject
{
public:
	Candle();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

};

