#include "PlayScene.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include"Define.h"
#include <stdlib.h> 
#include "SparkEffect.h"
#include "FireEffect.h"
#include "Ground.h"
#include "Zombie.h"
#include "Candle.h"
#include "Portal.h"
#include "Weapon.h"
#include "Axe.h"
#include "Boomerang.h"
#include "Dagger.h"
#include "Holywater.h"
#include "Knight.h"
#include "Bumper.h"
#include "StairBottom.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/




/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str()) * TILE_WIDTH;
	float y = atof(tokens[2].c_str())*TILE_HEIGHT + STATUS_BOARD_HEIGHT;

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case Object_Type::SIMON: 
			obj = player = new Simon();
		break;
	case Object_Type::GROUND: obj = new Ground(); break;
	case Object_Type::ZOMBIE: obj = new CZombie(); break;
	case Object_Type::KNIGHT:	obj = new Knight(); break;
	case Object_Type::CANDLE: obj = new Candle(); break;
	case Object_Type::BUMPER: obj = new Bumper();	break;
	case Object_Type::STAIR_BOTTOM: 
	{
		int stair_dir = atoi(tokens[4].c_str());
		int stair_type = atoi(tokens[5].c_str());
		int id = atoi(tokens[6].c_str());
		obj = new StairObject(stair_dir,stair_type,id); 
	}
		break;
	case Object_Type::PORTAL:
	{	
		int scene_id = atoi(tokens[3].c_str());

		obj  = new CPortal(scene_id);
	}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->isEnabled = true;
	if (ani_set_id != 0)
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
	}
	if(obj != NULL)
	switch (object_type)
	{
	case Object_Type::PORTAL:
	case Object_Type::GROUND:
	case Object_Type::CANDLE:
	case Object_Type::BUMPER:
	case Object_Type::STAIR_BOTTOM:
		objectList.push_back(obj);
		break;
	case Object_Type::ZOMBIE:
	case Object_Type::KNIGHT:
		enemyList.push_back(obj);
		break;
	default:
		break;
		
	}
}

void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;

	f.open(path);

	int ID;
	int rowMap, columnMap , columnTile, rowTile ,totalTiles;
	f >>ID>> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	//khoi tao ma tran data
	int** tileMapData = new int* [rowMap];
	for (int j = 0; j < rowMap; j++)
		tileMapData[j] = new int[columnMap];
	for (int i = 0; i < rowMap; i++)
		for (int j = 0; j < columnMap; j++)
			f >> tileMapData[i][j];
	f.close();
	
	
	tileMap = new Map(ID, rowMap, columnMap,rowTile, columnTile, totalTiles);
	tileMap->ExtractTileFromTileSet();
	tileMap->SetTileMapData(tileMapData);

}

void CPlayScene::LoadScene()
{
	
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[RESOURCES]") { section = SCENE_SECTION_RESOURCES; continue; }

		if (line == "[TILE_SET]") { section = SCENE_SECTION_TILESET; continue; }

		if (line == "[TILE_MAP_DATA]") { section = SCENE_SECTION_TILEMAP_DATA; continue; }


		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_RESOURCES: _ParseSection_RESOURCES(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILESET: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		}
	}

	f.close();

	
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\Texture\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	statusboard = new StatusBoard(player);
	statusboard->SetFont(CGame::GetInstance()->GetFont());
	statusboard->SetSceneId(id);
	camera = new Camera();
	camera->SetMapWidth(tileMap->GetMapWidth());
	tileMap->SetCamera(camera);
	CGame::GetInstance()->SetCamera(camera);
	LoadBackUpData();
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}



void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list

	for (UINT i = 0; i < effectList.size(); i++)
	{
		if (!effectList[i]->isFinished)
			effectList[i]->Update(dt);
		else
		{
			free(effectList[i]); 
			effectList[i] = NULL;
			effectList.erase(effectList.begin()+i);
		}
	}

	
	//update item
	for (UINT i = 0; i < itemList.size(); i++)
	{
		LPGAMEOBJECT item = itemList[i];
		if (!item->isEnabled)
		
		{
			free(itemList[i]);
			itemList[i] = NULL;
			itemList.erase(itemList.begin() + i);
		}
		else
		item->Update(dt, &objectList);
	}
	//update enemy
	for (UINT i = 0; i < enemyList.size(); i++)
	{
		
		if (enemyList[i]->isEnabled)
		{
			enemyList[i]->Update(dt, &objectList);
			if (enemyList[i]->isDestroyed)
			{
				CreateEffect(enemyList[i], Effect_Type::FIRE_EFFECT);
				enemyList[i]->isEnabled = false;
				SpawnItem(enemyList[i]);
			}
		}
		else
		{
		free(enemyList[i]);
		enemyList[i] = NULL;
		enemyList.erase(enemyList.begin() + i);
		}

	}
	
	if (player->GetMainWeapon()->isEnabled)
	{
		vector<LPGAMEOBJECT> coObjects;
		GetCollidableObject(player->GetMainWeapon(), coObjects);
		player->GetMainWeapon()->Update(dt, &coObjects);
	}
	
	if (player->GetSubWeapon() != NULL)
	{
	//	DebugOut(L"IsEnabled %d\n", player->GetSubWeapon()->isEnabled);
		if (player->GetSubWeapon()->isEnabled == true)
		{
			
			vector<LPGAMEOBJECT> coObjects;
			GetCollidableObject(player->GetSubWeapon(), coObjects);
			player->GetSubWeapon()->Update(dt, &coObjects);

		}
		if (camera->IsInCam(player->GetSubWeapon()) == false)
		{
			player->GetSubWeapon()->isEnabled = false;
		}

	}
	
	CheckForWeaponCollision();
	CheckForEnemyCollison();
	CheckForCollisonWithItems();
	// Update camer

	camera->Update(player);

	//DebugOut(L"Cx = %d , Cy = %d\n", cx, cy);

	statusboard->Update(dt);

	//update objects
	for (UINT i = 0; i < objectList.size(); i++)
	{
		if (objectList[i]->isEnabled)
		{
			objectList[i]->Update(dt, &objectList);
			if (objectList[i]->isDestroyed)
			{
				CreateEffect(objectList[i], Effect_Type::FIRE_EFFECT);
				objectList[i]->isEnabled = false;
				SpawnItem(objectList[i]);
			}
		}
		else
		{
			free(objectList[i]);
			objectList[i] = NULL;
			objectList.erase(objectList.begin() + i);
		}
	}
	player->Update(dt, &objectList);
}

void CPlayScene::Render()
{
	tileMap->Render();
	player->Render();
	for (int i = 0; i < objectList.size(); i++)
		if(objectList[i]->isEnabled)
			objectList[i]->Render();
	for (auto item : itemList)
		if (item->isEnabled)
			item->Render();
	for (int i = 0; i < enemyList.size(); i++)
		if (enemyList[i]->isEnabled)
			enemyList[i]->Render();
	for (auto effect : effectList)
	{
		if (!effect->isFinished)
			effect->Render();
	}

	if (player->GetSubWeapon() != NULL)
	{
		if (player->GetSubWeapon()->isEnabled)
			player->GetSubWeapon()->Render();
	}
	statusboard->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{

	for (int i = 0; i < objectList.size(); i++)
		delete objectList[i];
	objectList.clear();
	for (int i = 0; i < itemList.size(); i++)
		delete itemList[i];
	itemList.clear();
	for (int i = 0; i < enemyList.size(); i++)
		delete enemyList[i];
	enemyList.clear();
	for (int i = 0; i < effectList.size(); i++)
		delete effectList[i];
	effectList.clear();
	for(auto x : weaponList)
	{
		CWeapon* wp = x.second;
		delete wp;
	}
	weaponList.clear();
}

void CPlayScene::SpawnItem(LPGAMEOBJECT obj)
{
	float x, y;
	obj->GetPosition(x, y);
	Item* item = new Item();
	int type = Item_Type::CHICKEN;
	if (player->GetMainWeapon()->GetState() < WHIP_LEVEL2)
		type = Item_Type::CHAIN;
	else if (player->GetHeartsCollected() < 15)
		type = Item_Type::LARGEHEART;
	else if (player->GetSubWeapon() == NULL)
		type = Item_Type::DAGGER_ITEM;
	else
		type = rand() % 17;	//co 17 loai item
	item->SetState(type);
	item->SetPosition(x, y);
	itemList.push_back(item);
}

void CPlayScene::CreateEffect(LPGAMEOBJECT obj,int type)
{
	DebugOut(L"create effect\n");
	float x, y;
	obj->GetPosition(x, y);

		Effect* effect = NULL;
		switch (type)
		{
		case Effect_Type::FIRE_EFFECT:
			effect = new FireEffect(x, y);
			break;
		case Effect_Type::SPARK_EFFECT:
			effect = new SparkEffect(x, y);
			break;
		default:
			break;
		}
		if (effect != NULL)
		{
			effect->SetAnimationSet(CAnimationSets::GetInstance()->Get(type));
			effectList.push_back(effect);
		}
		else
			DebugOut(L"[ERROR]Wrong effect type");
	
	
}

void CPlayScene::CheckForWeaponCollision()
{
	/*if (player->IsAttacking() && player->animation_set->at(player->state)->IsRenderingLastFrame())
	{
		for (UINT i = 0; i < dynamicObjectList.size(); i++)
		{

			LPGAMEOBJECT obj = dynamicObjectList[i];
			if (obj->isEnabled)
			{
				if (this->player->GetMainWeapon()->IsColidingAABB(obj) == true)
				{
					if (dynamic_cast<Candle*>(obj))
					{
						Candle* e = dynamic_cast<Candle*> (obj);
						e->SetState(CANDLE_DESTROYED);
						SpawnItem(e);
						CreateEffect(e, Effect_Type::FIRE_EFFECT);

					}
					
				}
				
			}
		}
		for (UINT i = 0; i < enemyList.size(); i++)
		{
			LPGAMEOBJECT obj = enemyList[i];
			if (obj->isEnabled)
			{
				if (this->player->GetMainWeapon()->IsColidingAABB(obj) == true)
				{
					if (dynamic_cast<CZombie*>(obj))
					{
						CZombie* e = dynamic_cast<CZombie*> (obj);

						e->SetState(ZOMBIE_STATE_DIE);
						SpawnItem(e);
						CreateEffect(e,Effect_Type::FIRE_EFFECT);
					}
					
				}
				
			}
		}
	}
	if(player->GetSubWeapon() != NULL)
	if (player->GetSubWeapon()->isEnabled)
	{
		for (auto e : player->GetSubWeapon()->dynamicCoEvents)
		{
			LPGAMEOBJECT obj = e->obj;
			if (dynamic_cast<CZombie*>(obj))
			{
				obj->SetState(ZOMBIE_STATE_DIE);
				SpawnItem(obj);
				CreateEffect(obj, Effect_Type::FIRE_EFFECT);
			}
			else if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* e = dynamic_cast<Candle*> (obj);
				e->SetState(CANDLE_DESTROYED);
				SpawnItem(e);
				CreateEffect(e, Effect_Type::FIRE_EFFECT);
			}
			
			player->GetSubWeapon()->isEnabled = false;
		}
	}*/
}

void CPlayScene::CheckForEnemyCollison()
{
	if (!player->isInvulnerable)
		for (auto enemy : enemyList)
		{
			if(enemy->isEnabled)
				if (player->IsColidingAABB(enemy))
				{
					if(player->stairState ==0)
						player->SetState(SIMON_DAMAGED);
					player->AddHealth(-1);
				}
		}
}

void CPlayScene::CheckForCollisonWithItems()
{
	for (auto item : itemList)
	{
		if (item->isEnabled)
		{
			if (player->IsColidingAABB(item))
			{
				AccquireItem(item->GetState());
				item->isEnabled = false;

			}
		}
	}
}

void CPlayScene::AccquireItem(int type)
{
	BackUp* backup = BackUp::GetInstance();

	switch (type)
	{
	case Item_Type::AXE_ITEM:
		statusboard->SetSupweapon(type);
		backup->SetSupWPItem(type);
		SetSubWeapon(Weapon_Type::AXE);
		break;
	case Item_Type::BOOMERANG_ITEM:
		statusboard->SetSupweapon(type);
		backup->SetSupWPItem(type);
		SetSubWeapon(Weapon_Type::BOOMERANG);
		break;
	case Item_Type::CROSS:
		break;
	case Item_Type::DAGGER_ITEM:
		statusboard->SetSupweapon(type);
		backup->SetSupWPItem(type);
		SetSubWeapon(Weapon_Type::DAGGER);
		break;
	case Item_Type::HOLYWATER_ITEM:
		statusboard->SetSupweapon(type);
		backup->SetSupWPItem(type);
		SetSubWeapon(Weapon_Type::HOLYWATER);
		break;
	case Item_Type::CHAIN:
		player->SetState(SIMON_POWERUP);
		break;
	case Item_Type::INVISPOTION:
		player->StartInvisibilityTimer();
		break;
	case Item_Type::LARGEHEART:
		player->SetHeartsCollected(player->GetHeartsCollected() + 5);
		break;
	case Item_Type::SMALLHEART:
		player->SetHeartsCollected(player->GetHeartsCollected() + 1);
	case Item_Type::REDMONEYBAG:
	case Item_Type::WHITEMONEYBAG:
	case Item_Type::BLUEMONEYBAG:
		player->SetScore(player->GetScore() + 100);
		break;
	case Item_Type::CHICKEN:
		player->SetHP(SIMON_DEFAULT_HEALTH);
		break;
	}
}

void CPlayScene::SetSubWeapon(int type)
{
	if(weaponList[type]==NULL)
	{
		switch (type)
		{
		case Weapon_Type::AXE:
			weaponList[type] = new Axe();
			break;
		case Weapon_Type::BOOMERANG:
			weaponList[type] = new Boomerang();
			break;
		case Weapon_Type::DAGGER:
			weaponList[type] = new Dagger();
			break;
		/*case Weapon_Type::STOPWATCH:
			weaponList[type] = new Axe();
			break;*/
		case Weapon_Type::HOLYWATER:
			weaponList[type] = new Holywater();
			break;
		}
	}
	player->SetSubWeapon(weaponList[type]);
}

void CPlayScene::CleanUpObjects()
{
}

void CPlayScene::GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& coObjects)
{

	if (dynamic_cast<CWeapon*>(obj))
	{

		for (int i = 0; i < objectList.size(); i++)
		{
			if(objectList[i]->isEnabled)
				coObjects.push_back(objectList[i]);
		}
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->isEnabled)
				coObjects.push_back(enemyList[i]);
		}
		if(dynamic_cast<Boomerang*>(obj))
			coObjects.push_back(player);
	}
	
}

void CPlayScene::BackUpData()
{
	BackUp::GetInstance()->BackUpSimon(player);
	BackUp::GetInstance()->SetTime(statusboard->GetTime());
}


void CPlayScene::LoadBackUpData()
{
	BackUp* backup = BackUp::GetInstance();
	backup->LoadBackUp(player);
	statusboard->SetTime(backup->GetTime());
	AccquireItem(backup->GetSupWPItem());
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scene)->player;
	CGame* game = CGame::GetInstance();

	if (simon->GetState() == SIMON_DEAD)
		return;
	switch (KeyCode)
	{

	case DIK_1:
		game->SwitchScene(1);
		break;
	case DIK_2:
		game->SwitchScene(2);
		break;
	case DIK_3:
		game->SwitchScene(3);
		break;
	case DIK_4:
		game->SwitchScene(4);
		break;
	case DIK_0:
		game->SwitchScene(0);
		break;
	case DIK_Q:
		((CPlayScene*)scene)->AccquireItem(Item_Type::AXE_ITEM);
		break;
	case DIK_W:
		((CPlayScene*)scene)->AccquireItem(Item_Type::DAGGER_ITEM);
		break;
	case DIK_E:
		((CPlayScene*)scene)->AccquireItem(Item_Type::BOOMERANG_ITEM);
		break;
	case DIK_R:
		((CPlayScene*)scene)->AccquireItem(Item_Type::HOLYWATER_ITEM);
		break;
	case DIK_SPACE:
		if (simon->isJumping || simon->IsAttacking() || simon->GetState() == SIMON_SIT || simon->stairState !=0)
			return;
		simon->SetState(SIMON_JUMP);
		break;
	case DIK_X:
		if (simon->IsAttacking())
			return;
		if (simon->GetState() == SIMON_WALK)
			simon->SetState(SIMON_STAND);
		if (simon->GetState() == SIMON_STAND || simon->GetState() == SIMON_JUMP || simon->GetState() == SIMON_WALK)	
			simon->SetState(SIMON_STAND_ATTACK);
		
		else if (simon->GetState() == SIMON_SIT)
			simon->SetState(SIMON_SIT_ATTACK);
		
		else if (simon->GetState() == SIMON_STAIR_UP_IDLE)
			simon->SetState(SIMON_STAIR_UP_ATK);
		
		else if (simon->GetState() == SIMON_STAIR_DOWN_IDLE)
			simon->SetState(SIMON_STAIR_DOWN_ATK);

		if (game->IsKeyDown(DIK_UP))
		{
			if (simon->GetSubWeapon() != NULL)
			{
				if (simon->GetSubWeapon()->isEnabled == false)
					simon->canUseSubWeapon = true;
				else
					simon->canUseSubWeapon = false;
			}
		}
		else
			simon->canUseSubWeapon = false;

		if (simon->GetSubWeapon() != NULL && simon->canUseSubWeapon && simon->GetHeartsCollected()>0)
			simon->AttackWithSubWeapon();
		else simon->AttackWithWhip();


		//DebugOut(L"[INFO] Attack\n");
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scene)->player;

	if (simon->GetState() == SIMON_DEAD)
		return;
	if (simon->isWaitingForAni)
		return;
	if (simon->isJumping)
		return;
	
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->isCollidingStairObject&& simon->isTouchingGround && !simon->isAllowToGoDown && simon->GetCurrentStairType() == 1)
		{
			simon->GoToStairEnterPos();
		}

		else if (simon->isAllowToGoDown || simon->GetCurrentStairType() == 1 && simon->stairState != 0)
		{
			simon->SetDirection(simon->GetCurrentStairDirection());
			simon->SetState(SIMON_STAIR_DOWN);
		}
		else if(simon->GetCurrentStairType() == -1 && simon->stairState != 0)
		{
			simon->SetDirection(-simon->GetCurrentStairDirection());
			simon->SetState(SIMON_STAIR_DOWN);
		}

		else {
			simon->SetState(SIMON_SIT);
			if (game->IsKeyDown(DIK_RIGHT))
			{
				simon->SetDirection(-1);

			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
				simon->SetDirection(1);
			}
		}
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->stairState != 0)
		{
			if (simon->GetCurrentStairDirection() == -1)
			{
				simon->GoIntoStair(simon->GetCurrentStairType(), -1);
			}
			else
				simon->GoIntoStair(-simon->GetCurrentStairType(), -1);
		}
		else
		{
			simon->SetDirection(-1);
			simon->SetState(SIMON_WALK);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->stairState != 0)
		{
			if (simon->GetCurrentStairDirection() == 1)
			{
				simon->GoIntoStair(simon->GetCurrentStairType(), 1);
			}
			else
				simon->GoIntoStair(-simon->GetCurrentStairType(), 1);
		}
		else
		{
			simon->SetDirection(1);
			simon->SetState(SIMON_WALK);
		}
	}
	else {
		if (simon->stairState == STAIR_STATE_GOING_DOWN)
			simon->SetState(SIMON_STAIR_DOWN_IDLE);
		else if (simon->stairState == STAIR_STATE_GOING_UP)
			simon->SetState(SIMON_STAIR_UP_IDLE);
		else simon->SetState(SIMON_STAND);
	}
	if (game->IsKeyDown(DIK_UP))
	{
		if (simon->isCollidingStairObject && simon->isTouchingGround&& !simon->isAllowToGoUp && simon->GetCurrentStairType() == -1)
		{
			simon->GoToStairEnterPos();
		}
		else if (simon->isAllowToGoUp || simon->GetCurrentStairType() == -1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_UP, simon->GetCurrentStairDirection());

		else if (simon->GetCurrentStairType() == 1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_UP, -simon->GetCurrentStairDirection());



	}
}