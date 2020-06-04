#include "PlayScene.h"
#include <iostream>
#include <fstream>
#include<time.h>
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
#include "MovingPlatform.h"
#include "HunchBack.h"
#include "Ghost.h"
#include "Skeleton.h"
#include "StopWatch.h"
#include "Raven.h"
#include "Bat.h"
#include "BrickEffect.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath, int prevScene) :	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	id_counter = 0;
	weaponWaitingToBeProcess = -1;
	srand(time(NULL));
	deathTimer = -1;
	prev_scene = prevScene;
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
	int id = 0;
	switch (object_type)
	{
	case Object_Type::SIMON: 
	{
		if (player != NULL)
			return;
		int prev_scene_in_data = atoi(tokens[4].c_str());
		if (prev_scene_in_data != -1 && prev_scene_in_data != prev_scene)
			return;
		obj = player = new Simon();
	}
		break;
	case Object_Type::GROUND: 
	
		obj = new Ground();
		if(tokens.size() >4)
		{
			obj->isDestructable = true;
			obj->itemSpawn =  atoi(tokens[4].c_str());
		}
	
		break;
	case Object_Type::ZOMBIE: obj = new CZombie(); break;
	case Object_Type::KNIGHT:	obj = new Knight(); break;
	case Object_Type::HUNCHBACK:obj = new HunchBack();	break;
	case Object_Type::GHOST:	obj = new Ghost();	break;
	case Object_Type::BAT:	obj = new Bat();	break;
	case Object_Type::SKELETON: 
		obj = new Skeleton();	
		((Skeleton*)obj)->SetProjectTileList(&enemyProjectTile);
		((Skeleton*)obj)->screen_height = CGame::GetInstance()->GetScreenHeight();
		break;
	case Object_Type::RAVEN:	obj = new Raven();	break;
	case Object_Type::PLATFORM: obj = new MovingPlatform();	break;
	case Object_Type::CANDLE: 
	{
		int type = atoi(tokens[5].c_str());
		obj = new Candle();
		obj->SetState(type);
	}
		break;
	case Object_Type::BUMPER: obj = new Bumper();	break;
	case Object_Type::STAIR_OBJECT: 
	{
		int stair_dir = atoi(tokens[4].c_str());
		int stair_type = atoi(tokens[5].c_str());
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
	if (obj->isDestructable)
	{
		int item_spawn = atoi(tokens[4].c_str());

		obj->itemSpawn = item_spawn;
	}

	switch (object_type)
	{
	case Object_Type::ZOMBIE:
	case Object_Type::KNIGHT:	
	case Object_Type::HUNCHBACK:
	case Object_Type::GHOST:	
	case Object_Type::SKELETON:
	case Object_Type::RAVEN:
	case Object_Type::BAT:
	{
		int dam= atoi(tokens[5].c_str());
		obj->SetDamage(dam);
	}
		break;
	default:
		break;
	}
		
	// General object setup
	if (obj != NULL)
	{
		obj->SetPosition(x, y);
		obj->tag = object_type;
		obj->id = id_counter++;
		if (ani_set_id != 0)
		{
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
		}
		if (obj->isStatic)
			static_obj_grid->AddObject(obj);
		else
			dynamic_obj_grid->AddObject(obj);
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
			static_obj_grid = new Grid(tileMap->GetMapWidth(), tileMap->GetMapHeight());
			dynamic_obj_grid = new Grid(tileMap->GetMapWidth(), tileMap->GetMapHeight());
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
	camera->SetMapHeight(tileMap->GetMapHeight());
	tileMap->SetCamera(camera);
	CGame::GetInstance()->SetCamera(camera);
	LoadBackUpData();
	static_obj_grid->Update();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}



void CPlayScene::Update(DWORD dt)
{
	dynamic_obj_grid->Update();
	UpdateListsAccordingGrid();

	if (weaponWaitingToBeProcess != -1)
	{
		if (player->IsInRightFrameToUseSubWeapon() || player->GetSubWeaponType() == Item_Type::STOPWATCH_ITEM)
		{
			CreateSubWeapon(weaponWaitingToBeProcess);
			weaponWaitingToBeProcess = -1;
		}
	}


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
		enemyList[i]->isUpdatingFrame = !IsStopWatchEnabled();
		if (IsStopWatchEnabled()) continue;
		else
			if (enemyList[i]->isEnabled)
			{
				if (camera->IsInCam(enemyList[i]))
				{
					
					
					if (enemyList[i]->isDestroyed)
					{
						CreateEffect(enemyList[i], Effect_Type::FIRE_EFFECT);
						enemyList[i]->isEnabled = false;
						SpawnItem(enemyList[i]);
					}
					else {
						vector<LPGAMEOBJECT> coObject;
						for (UINT i = 0; i < objectList.size(); i++)
							coObject.push_back(objectList[i]);
						coObject.push_back(player);
						enemyList[i]->Update(dt, &coObject);
						coObject.clear();
					}
				}

			}

	}
	
	if (player->GetMainWeapon()->isEnabled)
	{
		vector<LPGAMEOBJECT> coObjects;
		GetCollidableObject(player->GetMainWeapon(), coObjects);
		player->GetMainWeapon()->Update(dt, &coObjects);
	}
	for (UINT i = 0; i < weaponList.size(); i++)
	{
		if (weaponList[i]->isEnabled)
		{
			if (weaponList[i]->tag == Weapon_Type::STOPWATCH)
				weaponList[i]->Update(dt);
			else if (camera->IsInCam(weaponList[i]))
			{
				vector<LPGAMEOBJECT> coObjects;
				GetCollidableObject(weaponList[i], coObjects);
				weaponList[i]->Update(dt, &coObjects);
			}
		}
		else

		{
			delete(weaponList[i]);
			weaponList[i] = NULL;
			weaponList.erase(weaponList.begin() + i);
		}
	}
	for (UINT i = 0; i < enemyProjectTile.size(); i++)
	{
		if (camera->IsInCam(enemyProjectTile[i]))
		{
			enemyProjectTile[i]->Update(dt);
		}
		else

		{
			free(enemyProjectTile[i]);
			enemyProjectTile[i] = NULL;
			enemyProjectTile.erase(enemyProjectTile.begin() + i);
		}
	}
	//DebugOut(L"weaponlist %d\n", weaponList.size());
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
			if (camera->IsInCam(objectList[i]))
			{
				objectList[i]->Update(dt, &objectList);
				if (objectList[i]->isDestroyed)
				{
					if (objectList[i]->tag == Object_Type::GROUND)
						CreateEffect(objectList[i], Effect_Type::BROKEN_BRICK_EFFECT);
					else
						CreateEffect(objectList[i], Effect_Type::FIRE_EFFECT);
					objectList[i]->isEnabled = false;
					if(objectList[i]->itemSpawn != Item_Type::NONE)
						SpawnItem(objectList[i]);
				}
			}
		}
		
	}
	
	player->Update(dt, &objectList);
	CheckSimonDead();
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
			if(camera->IsInCam(enemyList[i]))
				enemyList[i]->Render();
	for (auto effect : effectList)
	{
		if (!effect->isFinished)
			effect->Render();
	}

	for (auto subweapon : weaponList)
	{
		if(subweapon->tag!= Weapon_Type::STOPWATCH)
			subweapon->Render();
	}
	for (auto projectTile : enemyProjectTile)
		projectTile->Render();
	
	statusboard->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	objectList.clear();
	for (int i = 0; i < itemList.size(); i++)
		delete itemList[i];
	itemList.clear();
	enemyList.clear();
	for (int i = 0; i < effectList.size(); i++)
		delete effectList[i];
	effectList.clear();
	for (int i = 0; i < weaponList.size(); i++)
		delete weaponList[i];
	weaponList.clear();
	static_obj_grid->Unload();
	dynamic_obj_grid->Unload();
}

void CPlayScene::SpawnItem(LPGAMEOBJECT obj)
{
	float x, y;
	obj->GetPosition(x, y);
	Item* item = new Item();
	
	int type;
	if (obj->itemSpawn == Item_Type::RANDOM)
		type = rand() % 17;
	else
		type = obj->itemSpawn;

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
		case Effect_Type::BROKEN_BRICK_EFFECT:
			effect = new BrickEffect(x, y);
			((BrickEffect*)effect)->screen_height = CGame::GetInstance()->GetScreenHeight();
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


void CPlayScene::CheckForEnemyCollison()
{
	if (!player->isInvulnerable)
		for (auto enemy : enemyList)
		{
			if(enemy->isEnabled)
				if (player->IsColidingAABB(enemy))
				{
					if (player->stairState == 0)
						player->SetState(SIMON_DAMAGED);
					player->TakeDamage(enemy->damage);
					if (enemy->tag == Object_Type::RAVEN)
						enemy->isDestroyed = true;
					
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
	switch (type)
	{
	case Item_Type::AXE_ITEM:
	case Item_Type::BOOMERANG_ITEM:
	case Item_Type::STOPWATCH_ITEM:
	case Item_Type::DAGGER_ITEM:
	case Item_Type::HOLYWATER_ITEM:
		player->SetSubWeaponItem(type);
		break;
	case Item_Type::CROSS:
		flickerTimer = GetTickCount();
		DestroyAllOnScreenEnemy();
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
	case Item_Type::DOUBLESHOT:
		player->SetSupWeaponCap(2);
		break;
	case Item_Type::TRIPLESHOT:
		player->SetSupWeaponCap(3);
		break;

	}
}


void CPlayScene::CleanUpObjects()
{
	objectList.clear();
	enemyList.clear();
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
	CScene::LoadBackUpData();
}




void CPlayScene::UpdateListsAccordingGrid()
{
	CleanUpObjects();
	vector<LPGAMEOBJECT> temp_obj_list;
	
	static_obj_grid->GetObjectsAccordingCam(camera, &temp_obj_list);
	dynamic_obj_grid->GetObjectsAccordingCam(camera, &temp_obj_list);
	
	for(auto obj : temp_obj_list)
		switch (obj->tag)
		{
		case Object_Type::PORTAL:
		case Object_Type::GROUND:
		case Object_Type::CANDLE:
		case Object_Type::BUMPER:
		case Object_Type::STAIR_OBJECT:
		case Object_Type::PLATFORM:
			objectList.push_back(obj);
			break;
		case Object_Type::ZOMBIE:
		case Object_Type::KNIGHT:
		case Object_Type::HUNCHBACK:
		case Object_Type::GHOST:
		case Object_Type::RAVEN:
		case Object_Type::SKELETON:
		case Object_Type::BAT:
			enemyList.push_back(obj);
			break;
		default:
			break;
		}

}

void CPlayScene::SetWeaponToBeProcess(int type)
{
	weaponWaitingToBeProcess = type;
}

void CPlayScene::CreateSubWeapon(int type)
{
	LPWEAPON wp = NULL;
	switch (type)
	{
	case Item_Type::AXE_ITEM:
		wp = new Axe();
		break;
	case Item_Type::BOOMERANG_ITEM:
		wp = new Boomerang();
		break;
	case  Item_Type::DAGGER_ITEM:
		wp = new Dagger();
		break;
	case  Item_Type::STOPWATCH_ITEM:
		wp = new StopWatch();
		break;
	case Item_Type::HOLYWATER_ITEM:
		wp = new Holywater();
		break;
	}
	if (wp != NULL)
	{
		wp->Attack(player);
		weaponList.push_back(wp);
	
	}
	else
		DebugOut(L"Wrong weapon type\n");
}

void CPlayScene::CheckSimonDead()
{
	if (player->GetState() == SIMON_DEAD)
	{
		if (deathTimer == -1)
			deathTimer = GetTickCount();

		else if (GetTickCount() - deathTimer >= DEATH_TIMER_DELAY)
		{
			if (player->GetLife() == 0)
				CGame::GetInstance()->SwitchScene(INTRO_SCENE_ID);
			else
			{
				BackUp::GetInstance()->LoseLife();
				CGame::GetInstance()->SwitchScene(id);
			}
		}
	}
}

bool CPlayScene::IsStopWatchEnabled()
{
	for (auto weapon : weaponList)
		if (weapon->tag == Weapon_Type::STOPWATCH)
			return true;
	return false;
}


int CPlayScene::GetWeaponNum()
{
	return weaponList.size();
}

void CPlayScene::DestroyAllOnScreenEnemy()
{
	for (auto enemy : enemyList)
	{
		if (camera->IsInCam(enemy))
			enemy->isDestroyed = true;
	}
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
	case DIK_5:
		game->SwitchScene(5);
		break;
	case DIK_6:
		game->SwitchScene(6);
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
	case DIK_T:
		((CPlayScene*)scene)->AccquireItem(Item_Type::DOUBLESHOT);
		break;
	case DIK_Y:
		((CPlayScene*)scene)->AccquireItem(Item_Type::TRIPLESHOT);
		break;
	case DIK_U:
		((CPlayScene*)scene)->AccquireItem(Item_Type::CROSS);
		break;
	case DIK_I:
		((CPlayScene*)scene)->AccquireItem(Item_Type::STOPWATCH_ITEM);
		break;
	case DIK_M:
		((CPlayScene*)scene)->player->SetHP(0);
		break;
	case DIK_SPACE:
		if (simon->isJumping || simon->IsAttacking() || simon->GetState() == SIMON_SIT || simon->stairState !=0)
			return;
		simon->SetState(SIMON_JUMP);
		break;
	case DIK_X:
		if (simon->IsAttacking())
			return;

		if (game->IsKeyDown(DIK_UP) && simon->GetSubWeaponType() != -1 && ((CPlayScene*)scene)->GetWeaponNum() < simon->GetSubWeaponCap() )
		{
			if (simon->GetSubWeaponType() == Item_Type::STOPWATCH_ITEM && simon->GetHeartsCollected() >= 5)
			{
				if(((CPlayScene*)scene)->IsStopWatchEnabled())
					simon->AttackWithWhip();
				else
				{
					((CPlayScene*)scene)->SetWeaponToBeProcess(simon->GetSubWeaponType());
					return;
				}
			}
			else if( simon->GetSubWeaponType() != Item_Type::STOPWATCH_ITEM  && simon->GetHeartsCollected() > 0)
				((CPlayScene*)scene)->SetWeaponToBeProcess(simon->GetSubWeaponType());
		}
		else simon->AttackWithWhip();

		switch (simon->GetState())
		{
		case SIMON_WALK:
			simon->SetState(SIMON_STAND);
		case SIMON_STAND :
		case SIMON_JUMP:
			simon->SetState(SIMON_STAND_ATTACK);
			break;
		case SIMON_SIT:
			simon->SetState(SIMON_SIT_ATTACK);
			break;
		case SIMON_STAIR_DOWN:
			simon->SetStateToBeRender(SIMON_STAIR_DOWN_ATK);
			break;
		case SIMON_STAIR_DOWN_IDLE:
			simon->SetState(SIMON_STAIR_DOWN_ATK);
			break;
		case SIMON_STAIR_UP:
			simon->SetStateToBeRender(SIMON_STAIR_UP_ATK);
			break;
		case SIMON_STAIR_UP_IDLE:
			simon->SetState(SIMON_STAIR_UP_ATK);
			break;
		default:
			break;
		}
		


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
		simon->ProcessStair(STAIR_STATE_GOING_DOWN);
		
			
		/*DebugOut(L"iscoldingstair %d\n", simon->isCollidingStairObject);
		DebugOut(L"isTouchingGround %d\n", simon->isTouchingGround);
		DebugOut(L"isAllowToGoDown %d\n", !simon->isAllowToGoDown);
		DebugOut(L"CurrentStairType %d\n", simon->GetCurrentStairType());*/

		if (simon->isCollidingStairObject && !simon->isAllowToGoDown && simon->GetCurrentStairType() == STAIR_STATE_GOING_DOWN)
		{
			simon->GoToStairEnterPos();
		}

		else if (simon->isAllowToGoDown || simon->GetCurrentStairType() == 1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_DOWN, simon->GetCurrentStairDirection());
		else if(simon->GetCurrentStairType() == -1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_DOWN, -simon->GetCurrentStairDirection());
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
		if (simon->isSitting)
			return;
		simon->ProcessStair(STAIR_STATE_GOING_UP);
	/*	DebugOut(L"iscoldingstair %d\n", simon->isCollidingStairObject);
		DebugOut(L"isTouchingGround %d\n", simon->isTouchingGround);
		DebugOut(L"isAllowToGoUp %d\n", simon->isAllowToGoUp);
		DebugOut(L"CurrentStairType %d\n", simon->GetCurrentStairType());*/
		if (simon->isCollidingStairObject  && !simon->isAllowToGoUp && simon->GetCurrentStairType() == -1)
		{
			simon->GoToStairEnterPos();
		}
		else if (simon->isAllowToGoUp || simon->GetCurrentStairType() == -1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_UP, simon->GetCurrentStairDirection());

		else if (simon->GetCurrentStairType() == 1 && simon->stairState != 0)
			simon->GoIntoStair(STAIR_STATE_GOING_UP, -simon->GetCurrentStairDirection());



	}
}