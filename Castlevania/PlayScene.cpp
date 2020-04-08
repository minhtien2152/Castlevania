#include "PlayScene.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include"Define.h"
#include <stdlib.h> 


using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SECTION_UNKNOWN					-1
#define SCENE_SECTION_RESOURCES			0	
#define SCENE_SECTION_OBJECTS			1


#define RESOURCES_SECTION_TEXTURES		0
#define RESOURCES_SECTION_SPRITES		1
#define RESOURCES_SECTION_ANIMATIONS	2
#define RESOURCES_SECTION_ANIMATION_SETS 3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	string ani_id = tokens[0].c_str();
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
	DebugOut(L"[INFO] Added animation ID %s \n", ToWSTR(ani_id).c_str());
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		string ani_id = tokens[i].c_str();

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

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

		/*case OBJECT_TYPE_PORTAL:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;*/
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);



	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	switch (object_type)
	{
	case Object_Type::SIMON:
	case Object_Type::GROUND:
	case Object_Type::CANDLE:
		objectList.push_back(obj);
		break;
	case Object_Type::ZOMBIE:
		enemyList.push_back(obj);

	}
}

void CPlayScene::_ParseSection_RESOURCES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	LoadObjectResourses(path);
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	Effect::SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::LoadObjectResourses(LPCWSTR filepath)
{
	ifstream f;
	f.open(filepath);

	// current resource section flag
	int section = SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = RESOURCES_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = RESOURCES_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = RESOURCES_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = RESOURCES_SECTION_ANIMATION_SETS; continue;
		}
		if (line[0] == '[') { section = SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case RESOURCES_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case RESOURCES_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case RESOURCES_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case RESOURCES_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading object resources %s\n", filepath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	//update objects
	for (int i = 0; i < objectList.size(); i++)
	{
		LPGAMEOBJECT object = objectList[i];
		vector<LPGAMEOBJECT> coObjects;

		GetCollidableObject(object, coObjects);
		object->Update(dt, &coObjects);
	}
	//update item
	for (int i = 0; i < itemList.size(); i++)
	{
		LPGAMEOBJECT item = itemList[i];
		vector<LPGAMEOBJECT> coObjects;

		GetCollidableObject(item, coObjects);
		item->Update(dt, &coObjects);
	}
	//update enemy
	for (int i = 0; i < enemyList.size(); i++)
	{
		LPGAMEOBJECT enemy = enemyList[i];
		vector<LPGAMEOBJECT> coObjects;

		GetCollidableObject(enemy, coObjects);
		enemy->Update(dt, &coObjects);
	}

	//update effect
	for (auto effect : effectList)
		if(!effect->isFinished)
			effect->Update(dt);
	

	CheckForWeaponCollision();
	CheckForEnemyCollison();
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);


	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2-SIMON_BBOX_WIDTH;
	cy -= game->GetScreenHeight() / 2 - SIMON_BBOX_HEIGHT;

	CGame::GetInstance()->SetCamPos(cx,0);
	//DebugOut(L"Cx = %d , Cy = %d\n", cx, cy);
}

void CPlayScene::Render()
{
	for (int i = 0; i < objectList.size(); i++)
		objectList[i]->Render();
	for (int i = 0; i < itemList.size(); i++)
		itemList[i]->Render();
	for (int i = 0; i < enemyList.size(); i++)
		enemyList[i]->Render();
	for (auto effect : effectList)
		if (!effect->isFinished)
			effect->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objectList.size(); i++)
		delete objectList[i];

	objectList.clear();
	player = NULL;
}

void CPlayScene::SpawnItem(LPGAMEOBJECT obj)
{
	float x, y;
	obj->GetPosition(x, y);
	Item* item = new Item();
	int type = rand() % 17;
	item->SetState(type);
	item->SetPosition(x, y);
	itemList.push_back(item);
}

void CPlayScene::CreateEffect(LPGAMEOBJECT obj)
{
	DebugOut(L"create effect\n");
	float x, y;
	obj->GetPosition(x, y);
	Effect* effect = new SparkEffect(x, y);
	effectList.push_back(effect);
}

void CPlayScene::CheckForWeaponCollision()
{
	if (player->IsAttacking() && player->animation_set->at(player->state)->IsRenderingLastFrame())
	{
		for (UINT i = 0; i < objectList.size(); i++)
		{
			LPGAMEOBJECT obj = objectList[i];
			if (dynamic_cast<Candle*>(obj))
			{
				Candle* e = dynamic_cast<Candle*> (obj);
				if (this->player->GetMainWeapon()->IsColiding(e) == true)
				{
					e->SetState(CANDLE_DESTROYED);
					SpawnItem(e);
					CreateEffect(e);
				}
			}
		}
		for (UINT i = 0; i < enemyList.size(); i++)
		{
			LPGAMEOBJECT obj = enemyList[i];
			if (dynamic_cast<CZombie*>(obj) && obj->state != ZOMBIE_STATE_DIE)
			{
				CZombie* e = dynamic_cast<CZombie*> (obj);
				if (this->player->GetMainWeapon()->IsColiding(e) == true)
				{
					e->SetState(ZOMBIE_STATE_DIE);
					SpawnItem(e);
					CreateEffect(e);
				}
			}
		}
	}
}

void CPlayScene::CheckForEnemyCollison()
{
	for (auto enemy : enemyList)
	{
		if (player->IsColiding(enemy))
		{
			if (!player->isInvulerable)
				player->SetState(SIMON_DEFLECT);
			
		}
	}
}

void CPlayScene::GetCollidableObject(LPGAMEOBJECT obj, vector<LPGAMEOBJECT>& coObjects)
{
	if (dynamic_cast<Item*>(obj) || dynamic_cast<CZombie*>(obj) || dynamic_cast<Simon*>(obj))
	{
		for (int i = 0; i < objectList.size(); i++)
		{
			if (dynamic_cast<Ground*>(objectList[i]))
				coObjects.push_back(objectList[i]);
		}
	}

}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->player;
	
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->isJumping || simon->IsAttacking() || simon->GetState() == SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
		DebugOut(L"[INFO] JUMP \n");
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
		DebugOut(L"[INFO] Attack\n");
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
	if (simon->GetState() == SIMON_DEFLECT&& simon->animation_set->at(SIMON_DEFLECT)->IsOver() == false)
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