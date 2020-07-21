#pragma once
#include"SmartEnemy.h"
#include "Camera.h"
class BossBat: public SmartEnemy
{
	float destinationX;
	float destinationY;
	

	Camera* cam;
	
	bool isCharging;
	bool isHovering;
	bool justShooted;
	float a;
	vector<CWeapon*>* projectTileList;
public:
	BossBat();
	int screen_width;
	int screen_height;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	void SetCam(Camera* camera);
	bool IsPlayerWithinAttackRange();
	void PickRandomPositioning();
	void SetSpeed(float destinationX, float destinationY);
	bool IsAtDestination();
	void Hover();
	void Charge();
	void CalculateFlyingEquation();
	void HandleGoingOutOfScreen();
	void Shoot();
	bool IsLowerThanSimon();
	void SetProjectTileList(vector<CWeapon*>* projectTileList);


};

