#pragma once

#include <string>

using namespace std;

class Level {
public:
	Level(string name);
	~Level();
	string name();

private:
	string _name;
};
