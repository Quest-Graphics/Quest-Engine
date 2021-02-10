#include "Level.h"

Level::Level(std::string name)
{
	_name = name;
}

Level::~Level()
{
}

string Level::name()
{
	return _name;
}
