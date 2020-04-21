#include "HealthBar.h"
#include"Define.h"

#define HEALTH_CELL_ID 9
HealthBar::HealthBar(float x, float y, LPGAMEOBJECT object,int type)
{
	this->x = x;
	this->y = y;
	this->object = object;
	currHP =  this->object->GetHP();
	
	this->type = type;
	if (type == SIMON_HEALTH)
		defaultHP = SIMON_DEFAULT_HEALTH;
	animation_set = CAnimationSets::GetInstance()->Get(HEALTH_CELL_ID);
}

HealthBar::~HealthBar()
{
}

void HealthBar::Update()
{
	currHP = object->GetHP();
}



void HealthBar::Render()
{
	
		int i, j;
	
		for (i = 0; i < currHP; i++)
			animation_set->at(type)->Render(x + i * HEALTH_FRAME_WIDTH , y, 1,false);//always stay on screen -> accordingCam = false

		for (j = i; j < defaultHP; j++)
			animation_set->at(MISSING_HEALTH)->Render(x + j * HEALTH_FRAME_WIDTH, y, 1,false);
		
	
}
