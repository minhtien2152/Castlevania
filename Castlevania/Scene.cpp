#include "Scene.h"
#include"Utils.h"
#include "Define.h"
#include <fstream>
CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	player = NULL;
}

void CScene::LoadObjectResourses(LPCWSTR filepath)
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

void CScene::_ParseSection_RESOURCES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	LoadObjectResourses(path);
}

void CScene::_ParseSection_TEXTURES(string line)
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

void CScene::_ParseSection_SPRITES(string line)
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

void CScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	string ani_id = tokens[0].c_str();
	for (UINT i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
	DebugOut(L"[INFO] Added animation ID %s \n", ToWSTR(ani_id).c_str());
}

void CScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (UINT i = 1; i < tokens.size(); i++)
	{
		string ani_id = tokens[i].c_str();

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CScene::LoadBackUpData()
{
	BackUp* backup = BackUp::GetInstance();
	backup->LoadBackUp(player);
	statusboard->SetTime(backup->GetTime());
}

bool CScene::IsFlickering()
{
	if (GetTickCount() - flickerTimer >= CROSS_FLICKER_TIME)
		return false;
	return true;
}

void CScene::StartCalculatingHighScore()
{
	this->IsCalculatingHighScore = true;
	statusboard->IsCalculatingHighScore = true;
	player->SetState(SIMON_STAND);
}

