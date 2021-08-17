#pragma once

struct triangle;

struct vec3d
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

vec3d Vector_Add(vec3d& v1, vec3d& v2);

vec3d Vector_Sub(vec3d& v1, vec3d& v2);

vec3d Vector_Mul(vec3d& v1, float k);

vec3d Vector_Div(vec3d& v1, float k);

float Vector_DotProduct(vec3d& v1, vec3d& v2);

float Vector_Length(vec3d& v);

vec3d Vector_Normalise(vec3d& v);

//Calculate the Normal of a Traingle
vec3d Vector_Normal(vec3d& p1, vec3d& p2, vec3d& p3);

//Return the vertex closest to a plane
vec3d Vector_CloseToPlane(vec3d& normal, triangle& tri);

vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);

vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd, float& t);
