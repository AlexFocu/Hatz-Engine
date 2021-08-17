#pragma once

#include "olcPixelGameEngine.h"

#include "vec2d.h"
#include "vec3d.h"

struct triangle
{
	vec3d p[3];
	vec2d t[3];

	//Store the texture of the Triangle
	olc::Sprite* tex = nullptr;

	//Store the color of the Triangle
	olc::Pixel col;
};

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);

float Triangle_DistanceToAPoint(vec3d& plane_n, vec3d& plane_p, vec3d& p);

float Triangle_Area(triangle& tri);

bool Triangle_InsidePoint(triangle& tri, olc::vi2d& cam);
