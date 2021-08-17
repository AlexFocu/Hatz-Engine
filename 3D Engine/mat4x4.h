#pragma once

struct vec3d;
struct triangle;
struct mesh;

struct mat4x4
{
	float m[4][4] = { 0 };
};

//Calculate World Matrix for a Mesh
mat4x4 Matrix_WorldMatrix(mesh& m);

//Transform a Triangle with a Matrix
triangle Matrix_Transform(mat4x4 mat, triangle tri);

vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i);

mat4x4 Matrix_MakeIdentity();

mat4x4 Matrix_MakeRotationX(float fAngleRad);

mat4x4 Matrix_MakeRotationY(float fAngleRad);

mat4x4 Matrix_MakeRotationZ(float fAngleRad);

mat4x4 Matrix_MakeTranslation(float x, float y, float z);

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);

mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);

mat4x4 Matrix_QuickInverse(mat4x4& m);
