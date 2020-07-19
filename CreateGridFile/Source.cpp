
#include<iostream>
#include<vector>
#include<fstream>
#include <sstream>
#include"dirent.h"
using namespace std;

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 504

struct BOX
{
	float l, t, r, b;
};

int GetColumn(float x)
{
	return x / (SCREEN_WIDTH/2);
}
int GetRow(float y)
{
	return y / (SCREEN_HEIGHT/2);
}

vector<pair<int,int>> PlaceObjectInGrid(BOX box)
{
	vector<pair<int, int>> result;
	int start_row = GetRow(box.t);
	int end_row = GetRow(box.b);
	int start_col = GetColumn(box.l);
	int end_col = GetColumn(box.r);
	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
		{
			result.push_back(make_pair(row, col));
		}
	return result;
}

vector<string> split(string line, string delimeter = "\t")
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}


void fdbdf(string path, string pathout) {
	ifstream f(path);
	ofstream fwrite(pathout);
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair, _direct;
	char str[1000];
	vector<pair<int, int>> temp;
	string resuft;
	while (f.getline(str, 1000))
	{
		resuft.clear();
		string line(str);

		vector<string> tokens = split(line);
		
		if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		float x = atof(tokens[1].c_str()) * 32;
		float y = atof(tokens[2].c_str()) * 32 + 80;
		temp.clear();
		BOX box;
		box.l = x;
		box.t = y;
		box.r = x + 32;
		box.b = y + 32;
		temp = PlaceObjectInGrid(box);

		std::stringstream ss;

		ss << temp.size();

		std::string sService = ss.str();

		
		for (int i = 0; i < 4; i++)
		{
			resuft += tokens[i];
			resuft += '\t';
		}
		resuft += sService;
		for (int i = 0; i < temp.size(); i++)
		{
			resuft += '\t';
			resuft += std::to_string(temp.at(i).first);
			resuft += '\t';
			resuft += std::to_string(temp.at(i).second);
		}
		for (int i = 4; i < tokens.size(); i++)
		{
			resuft += '\t';
			resuft += tokens[i];
		}
		fwrite << resuft << '\n';
	}
	f.close();
	fwrite.close();
}


int main() {
	while (true)
	{
	string path;
	cout << "Nhap duong dan map game: ";
	cin >> path;
	string pathout;
	cout << "Nhap duong dan file luu: ";
	cin >> pathout;
	fdbdf(path, pathout);
	}
	return 0;
}