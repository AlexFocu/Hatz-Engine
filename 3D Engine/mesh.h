#pragma once

#include "triangle.h"

#include <vector>
#include <string>

struct mesh
{
	//mesh(std::string sTag, olc::Sprite* sprTex);

	std::vector<triangle> tris;

	vec3d positionInScene;

	//Tag of the mesh
	std::string tag = "Default";

	//Texture of the mesh
	olc::Sprite* tex;

	bool LoadFromObjectFile(std::string sFileName, bool bHasTexture = false);
};
