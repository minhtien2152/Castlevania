#pragma once

#include "GameObject.h"

class Item : public CGameObject
{
	DWORD appearTime;   

public:
	Item();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects = NULL, vector<LPGAMEOBJECT>* dynamicCoObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	LPSPRITE GetCurrentSprite();
};

