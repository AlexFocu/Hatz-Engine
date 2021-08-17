#pragma once

#include "olcPixelGameEngine.h"
#include "HatzEngine-GUI.h"

#include "mat4x4.h"
#include "mesh.h"

class HatzEngine : public olc::PixelGameEngine
{
public:

	HatzEngine();

public:

	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;

private:

	//temp!!
	bool debug = true;

	olc::Sprite* sprTest;
	olc::Sprite* sprTest2;
	mesh meshCube;
	mesh meshTest;

	//Store Triangles that needs to be drawn
	std::vector<triangle> vecTrianglesToRaster;

	//List of GameOjects in the Scene
	std::vector<mesh> vecMeshesInScene;

	//Matrix that converts from view space to screen space
	mat4x4 matProj;

	float* pDepthBuffer = nullptr;

	//Location of camera/player in world space
	vec3d vCamera;

	//Direction vector along the direction camera points
	vec3d vLookDir;

	//View matrix from camera
	mat4x4 matView;

	//FPS Camera rotation in XZ plane
	float fYaw;

	//Spins World transform
	//float fTheta;

	//Player graphics
	mesh meshPlayer;

	//Velocity of the Player
	vec3d vVelocity;

	//Speed of the Player
	float fSpeed = 8.0f;

	//Gravity of the Player
	float fGravity = -9.81f;

	//Check to see if the Player is grounded
	bool bIsGrounded = true;

	//GUI
	GUI gui;

private:

	bool Engine_MouseOver();

	void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
						  int x2, int y2, float u2, float v2, float w2,
				    	  int x3, int y3, float u3, float v3, float w3,
						  olc::Sprite* tex);

	//Get input and move the Player
	void Player_Move(float fRotationSpeed = 2.0f, float fJumpHeight = 3.0f);

	//Apply gravity to the Player
	void Player_ApplyGravity();

	//Wait for secounds
	void Engine_WaitForSeconds(float fTime);

	//Set position of a GameObject
	void Engine_SetPosition(mesh& m, float x, float y, float z);

	//Instantiate a GameObject
	void Engine_Instantiate(mesh& meshInst);

	//Check to see if 2 Game Objects overlap
	bool Engine_OverlapGO(mesh& mesh1, mesh& mesh2);

	//Move Camera in Space / Move player (it needs to be called only once in Main.cpp)
	void Engine_MoveCamera();

	//Draw all GameObjects (it needs to be called only once in Main.cpp)
	void Engine_DrawGO();
};