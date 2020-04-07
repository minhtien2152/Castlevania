#pragma once
#include "Define.h"
#include "GameObject.h"

#include"AnimationSets.h"
#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2

class Whip: public CGameObject
{

public:
	Whip();
	~Whip();
	void Attack(int nx,bool isSitting);
	void SetPosition(int x, int y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render() ;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

