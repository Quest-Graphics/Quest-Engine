#pragma once

#include "tiny_obj_loader.h"

class Entity {
public:
	Entity()
	{}

private:
	static std::map<std::string, std::vector<tinyobj::shape_t>> objCache;

	static std::vector<tinyobj::shape_t> loadObj(std::string file)
	{

	}
};
