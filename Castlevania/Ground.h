#pragma once
#include "GameObject.h"

class Ground : public CGameObject
{
public:
	Ground();
	~Ground();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};