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
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case Object_Type::SIMON:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] Simon object was created before! ");
			return;
		}
		obj = new Simon();
		player = (Simon*)obj;
		break;
	case Object_Type::GROUND: obj = new Ground(); break;
	case Object_Type::ZOMBIE: obj = new CZombie(); break;
	case Object_Type::CANDLE: obj = new Candle(); break;

	case Object_Type::PORTAL:
	{	
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->isEnabled = true;


	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	switch (object_type)
	{
	case Object_Type::GROUND:
		staticObjectList.push_back(obj);
		break;
	case Object_Type::SIMON:
	case Object_Type::CANDLE:
		dynamicObjectList.push_back(obj);
		break;
	case Object_Type::ZOMBIE:
		enemyList.push_back(obj);
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

	
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	Effect::SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
	statusboard = new StatusBoard(player);
	statusboard->SetFont(CGame::GetInstance()->GetFont());
	camera = new Camera();
	camera->SetMapWidth(tileMap->GetMapWidth());
	tileMap->SetCamera(camera);
	CGame::GetInstance()->SetCamera(camera);
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}



void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	//update objects
	for (UINT i = 0; i < dynamicObjectList.size(); i++)
	{
		LPGAMEOBJECT object = dynamicObjectList[i];
		vector<LPGAMEOBJECT> staticCoObjects;
		vector<LPGAMEOBJECT> dynamicCoObjects;
		GetCollidableObject(object, staticCoObjects, dynamicCoObjects);
		object->Update(dt, &staticCoObjects,& dynamicCoObjects);
	}
	//update item
	for (UINT i = 0; i < itemList.size(); i++)
	{
		LPGAMEOBJECT item = itemList[i];
		if (!item->isEnabled)
			continue;
		vector<LPGAMEOBJECT> staticCoObjects;
		vector<LPGAMEOBJECT> dynamicCoObjects;
		GetCollidableObject(item, staticCoObjects, dynamicCoObjects);
		item->Update(dt, &staticCoObjects,&dynamicCoObjects);
	}
	//update enemy
	for (UINT i = 0; i < enemyList.size(); i++)
	{
		LPGAMEOBJECT enemy = enemyList[i];
		vector<LPGAMEOBJECT> staticCoObjects;
		vector<LPGAMEOBJECT> dynamicCoObjects;
		GetCollidableObject(enemy, staticCoObjects, dynamicCoObjects);
		enemy->Update(dt, &staticCoObjects, &dynamicCoObjects);
	}

	//update effect
	for (auto effect : effectList)
		if(!effect->isFinished)
			effect->Update(dt);
	
	if (player->GetSubWeapon() != NULL)
	{
		if (player->GetSubWeapon()->isEnabled == true)
		{
			vector<LPGAMEOBJECT> staticCoObjects;
			vector<LPGAMEOBJECT> dynamicCoObjects;
			GetCollidableObject(player->GetSubWeapon(), staticCoObjects, dynamicCoObjects);
			player->GetSubWeapon()->Update(dt, &staticCoObjects, &dynamicCoObjects);

		}
		if (camera->IsInCam(player->GetSubWeapon()) == false)
		{
			
			player->GetSubWeapon()->isEnabled = false;
			//player->isUsingSubWeapon == false;
		}

	}

	CheckForWeaponCollision();
	CheckForEnemyCollison();
	CheckForCollisonWithItems();
	// Update camera
	float cx, cy;
	player->GetPosition(cx, cy);


	camera->Update(player);

	//DebugOut(L"Cx = %d , Cy = %d\n", cx, cy);

	statusboard->Update(dt);
}

void CPlayScene::Render()
{
	tileMap->Render();

	for (int i = 0; i < dynamicObjectList.size(); i++)
		if(dynamicObjectList[i]->isEnabled)
			dynamicObjectList[i]->Render();
	for (auto item : itemList)
		if (item->isEnabled)
			item->Render();
	for (int i = 0; i < enemyList.size(); i++)
		if (enemyList[i]->isEnabled)
			enemyList[i]->Render();
	for (auto effect : effectList)
		if (!effect->isFinished)
			effect->Render();

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
	for (int i = 0; i < dynamicObjectList.size(); i++)
		delete dynamicObjectList[i];
	for (int i = 0; i < staticObjectList.size(); i++)
		delete staticObjectList[i];
	staticObjectList.clear();
	dynamicObjectList.clear();
	player = NULL;
}

void CPlayScene::SpawnItem(LPGAMEOBJECT obj)
{
	float x, y;
	obj->GetPosition(x, y);
	Item* item = new Item();
	int type = Item_Type::DAGGER_ITEM;
	//if (player->GetMainWeapon()->GetState() < WHIP_LEVEL2)
	//	type = Item_Type::CHAIN;
	//else if (player->GetHeartsCollected() < 15)
	//	type = Item_Type::LARGEHEART;
	//else if (player->GetSubWeapon() == NULL)
	//	type = Item_Type::DAGGER_ITEM;
	//else
	//	type = rand() % 17;	//co 17 loai item
	item->SetState(type);
	item->SetPosition(x, y);
	itemList.push_back(item);
}

void CPlayScene::CreateEffect(LPGAMEOBJECT obj)
{
	DebugOut(L"create effect\n");
	float x, y;
	obj->GetPosition(x, y);
	Effect* effect = new FireEffect(x, y);
	effectList.push_back(effect);
}

void CPlayScene::CheckForWeaponCollision()
{
	if (player->IsAttacking() && player->animation_set->at(player->state)->IsRenderingLastFrame())
	{
		for (UINT i = 0; i < dynamicObjectList.size(); i++)
		{

			LPGAMEOBJECT obj = dynamicObjectList[i];
			if (obj->isEnabled)
				if (this->player->GetMainWeapon()->IsColiding(obj) == true)
				{
					if (dynamic_cast<Candle*>(obj))
					{
						Candle* e = dynamic_cast<Candle*> (obj);
						e->SetState(CANDLE_DESTROYED);
						SpawnItem(e);
						CreateEffect(e);

					}
				}
		}
		for (UINT i = 0; i < enemyList.size(); i++)
		{
			LPGAMEOBJECT obj = enemyList[i];
			if (obj->isEnabled)
			{
				if (dynamic_cast<CZombie*>(obj))
				{
					CZombie* e = dynamic_cast<CZombie*> (obj);
					if (this->player->GetMainWeapon()->IsColiding(e) == true)
					{
						e->SetState(ZOMBIE_STATE_DIE);
						SpawnItem(e);
						CreateEffect(e);
					}
				}
				obj->isEnabled = false;
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
				CreateEffect(obj);
			}
			else if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* e = dynamic_cast<Candle*> (obj);
				e->SetState(CANDLE_DESTROYED);
				SpawnItem(e);
				CreateEffect(e);
			}
			obj->isEnabled = false;
			player->GetSubWeapon()->isEnabled = false;
		}
	}
}

void CPlayScene::CheckForEnemyCollison()
{
	if (!player->isInvulnerable)
		for (auto enemy : enemyList)
		{
			if(enemy->isEnabled)
			if (player->IsColiding(enemy))
			{
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
			if (player->IsColiding(item))
			{
				switch (item->GetState())
				{
				case Item_Type::AXE_ITEM:
					statusboard->SetSupweaponSprite(item->GetCurrentSprite());
					player->SetSubWeapon(Weapon_Type::AXE);
					break;
				case Item_Type::BOOMERANG_ITEM:
					break;
				case Item_Type::CROSS:
					break;
				case Item_Type::DAGGER_ITEM:
					statusboard->SetSupweaponSprite(item->GetCurrentSprite());
					player->SetSubWeapon(Weapon_Type::DAGGER);
					break;
				case Item_Type::HOLYWATER_ITEM:
					statusboard->SetSupweaponSprite(item->GetCurrentSprite());
					player->SetSubWeapon(Weapon_Type::HOLYWATER);
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
					break;
				}
				item->isEnabled = false;

			}
		}
	}
}

void CPlayScene::GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& staticCoObjects, vector<LPGAMEOBJECT>& dynamicCoObjects)
{
	for (int i = 0; i < staticObjectList.size(); i++)
	{
		if (dynamic_cast<Ground*>(staticObjectList[i]))
			staticCoObjects.push_back(staticObjectList[i]);			//hien tai tat ca object deu tuong tac voi ground
	}
	
	if (dynamic_cast<SubWeapon*>(obj))
	{

		for (int i = 0; i < dynamicObjectList.size(); i++)
		{
			if(dynamicObjectList[i]->isEnabled)
				if (dynamic_cast<Candle*>(dynamicObjectList[i]))
					dynamicCoObjects.push_back(dynamicObjectList[i]);
		}
		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->isEnabled)
				dynamicCoObjects.push_back(enemyList[i]);
		}
	}

}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->player;
	CGame* game = CGame::GetInstance();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->isJumping || simon->IsAttacking() || simon->GetState() == SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
		break;
	case DIK_X:
		if (simon->IsAttacking())
			return;
		if (simon->GetState() == SIMON_WALK)
			simon->SetState(SIMON_STAND);
		if (simon->GetState() == SIMON_STAND || simon->GetState() == SIMON_JUMP || simon->GetState() == SIMON_WALK)
		{
			simon->SetState(SIMON_STAND_ATTACK);
		}
		else if (simon->GetState() == SIMON_SIT)
		{
			simon->SetState(SIMON_SIT_ATTACK);
		}
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

		if (simon->GetSubWeapon() != NULL && simon->canUseSubWeapon)
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
	Simon* simon = ((CPlayScene*)scence)->player;

	if (simon->GetState() == SIMON_STAND_ATTACK && simon->animation_set->at(SIMON_STAND_ATTACK)->IsOver() == false)
		return;
	if (simon->GetState() == SIMON_SIT_ATTACK && simon->animation_set->at(SIMON_SIT_ATTACK)->IsOver() == false)
		return;//
	if (simon->GetState() == SIMON_DAMAGED && simon->animation_set->at(SIMON_DAMAGED)->IsOver() == false)
		return;//
	if (simon->GetState() == SIMON_POWERUP && simon->animation_set->at(SIMON_POWERUP)->IsOver() == false)
		return;//
	if (simon->isJumping)
		return;


	if (game->IsKeyDown(DIK_DOWN))
	{
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
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetDirection(-1);
		simon->SetState(SIMON_WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetDirection(1);
		simon->SetState(SIMON_WALK);
	}
	else simon->SetState(SIMON_STAND);
}