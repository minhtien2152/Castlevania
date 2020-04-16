#pragma once
#include "Define.h"
#include "GameObject.h"

#include"AnimationSets.h"
#define WHIP_LEVEL0 0
#define WHIP_LEVEL1 1
#define WHIP_LEVEL2 2

class Whip: public CGameObject
{

public:
	Whip();
	~Whip();
	void Attack(int nx,bool isSitting);
	void SetPosition(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render() ;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void LevelUp();
};

