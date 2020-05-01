#include "IntroScene.h"
#include "Textures.h"
#include "Define.h"
#include <fstream>

#define INTRO_ANIMATION_SET_ID 2
#define CASTLE_BACKGROUND_ID 2
#define MENU_ID 0
#define MENU_BAT_ID 1

#define INTRO_CASTLE_WAIT_TIME 3000
#include "Ground.h"
CIntroScene::CIntroScene(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
	game = CGame::GetInstance();
}
void CIntroScene::LoadScene()
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

		if (line == "[TEXTURES]") { section = RESOURCES_SECTION_TEXTURES; continue; }
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
		case RESOURCES_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		}
	}

	f.close();

	statusboard = new StatusBoard(player);
	statusboard->SetSceneId(id);
	statusboard->SetFont(CGame::GetInstance()->GetFont());
	camera = new Camera();
	camera->SetMapWidth(CGame::GetInstance()->GetScreenWidth());
	CGame::GetInstance()->SetCamera(camera);
	font = CGame::GetInstance()->GetFont();
	animation_set = CAnimationSets::GetInstance()->Get(INTRO_ANIMATION_SET_ID);
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Update(DWORD dt)
{


	if (game->GetState() == GAME_STATE_INTROWALK)
	{
		player->Update(dt);
		float posX, posY;
		player->GetPosition(posX, posY);
		if (posX >= game->GetScreenWidth() /2-20)
		{
			player->SetDirection(1);
			player->SetState(SIMON_WALK);
		}
		else
		{
			player->SetState(SIMON_WAIT);
			if (timeAccumulated >= INTRO_CASTLE_WAIT_TIME)
			{
				CGame::GetInstance()->SetState(GAME_STATE_PLAYSCENE);
				CGame::GetInstance()->SwitchScene(1);
			}
			else timeAccumulated += dt;
		}
		//DebugOut(L"State %d, posX %f,width %d\n", simon->GetState(), posX, SCREEN_WIDTH / 2);
	}

}

void CIntroScene::Render()
{

	if (game->GetState() == GAME_STATE_INTROWALK )
	{

		animation_set->at(CASTLE_BACKGROUND_ID)->Render(0, STATUS_BOARD_HEIGHT);	//background lau dai
		player->Render();
		statusboard->Render();
	
	}
	else
	{
		RECT rect;
		SetRect(&rect, 0, 220, SCREEN_WIDTH, 240);
		font->DrawTextW(NULL, L"Press any key", -1, &rect, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
		
		
		animation_set->at(MENU_ID)->Render(0, 0);
		animation_set->at(MENU_BAT_ID)->Render(368, 208);
		

	}
}

void CIntroScene::Unload()
{
	player = NULL;
}

void CIntroScene::_ParseSection_OBJECTS(string line)
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
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
}


void CIntroScenceKeyHandler::KeyState(BYTE* states)
{
}

void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	switch (KeyCode)
	{
	default:
		if (game->GetState() == GAME_STATE_MENU)
			game->SetState(GAME_STATE_INTROWALK);
		break;
	}

}

void CIntroScenceKeyHandler::OnKeyUp(int KeyCode)
{
}
