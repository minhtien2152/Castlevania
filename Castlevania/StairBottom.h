#pragma once
#include "GameObject.h"
class StairObject:public CGameObject
{
	int type;

public:
	StairObject(int dir,int type);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	float GetEnterPosX();
	bool IsInRightPosToEnterStair(float simonX);
	int GetType();
};

