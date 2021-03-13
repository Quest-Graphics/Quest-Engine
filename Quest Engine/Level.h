#pragma once

#include <string>

class Level {
public:
	Level(std::string name)
	{
		_name = name;
	}

	std::string name()
	{
		return _name;
	}

private:
	std::string _name;
};
