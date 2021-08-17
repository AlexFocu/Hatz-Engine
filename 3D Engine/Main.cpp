#include "olcPixelGameEngine.h"
#include "HatzEngine.h"

//de sters
#include <iostream>

bool HatzEngine::OnUserCreate()
{
	//!!TEMP!!

	meshCube.LoadFromObjectFile("assets/3D Models/test.obj", true);
	sprTest = new olc::Sprite("assets/Textures/wood.png");
	meshCube.tex = sprTest;

	meshTest.LoadFromObjectFile("assets/3D Models/cube.obj", true);
	sprTest2 = new olc::Sprite("assets/Textures/pattern.png");
	meshTest.tex = sprTest2;

	//temp
	Engine_SetPosition(meshCube, 0.0f, 0.0f, 6.0f);
	Engine_SetPosition(meshTest, 0.0f, 5.0f, 6.0f);

	//Projection Matrix
	matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

	//Depth Buffer
	pDepthBuffer = new float[ScreenWidth() * ScreenHeight()];

	gui.Create();

	return true;
}

bool HatzEngine::OnUserUpdate(float fElapsedTime)
{

	if (!IsFocused())
	{
		//Clear the Screen
		Clear(olc::BLACK);

		//Text
		DrawString(ScreenWidth()/6, ScreenHeight()/6, "The game is minimized!");

		return true;
	}

	//temp
	if (GetKey(olc::Key::I).bPressed)
		debug = !debug;

	if (debug)
	{
		gui.Run(meshCube.tag, meshCube.tag, &meshCube.positionInScene.x, meshCube.positionInScene.y, meshCube.positionInScene.z);
		/*std::cout << std::endl;
		std::cout << "x: " << meshCube.positionInScene.x << std::endl;
		std::cout << "y: " << meshCube.positionInScene.y << std::endl;
		std::cout << "z: " << meshCube.positionInScene.z << std::endl;*/
	}

	//Get input and move the Player
	Player_Move();

	//Move Camera in Space / Move player 
	Engine_MoveCamera();

	//DEBUG
	if (GetKey(olc::Key::P).bPressed)
	{
		std::cout << "mesh" << std::endl;
		std::cout << "x: " << meshPlayer.positionInScene.x << std::endl;
		std::cout << "y: " << meshPlayer.positionInScene.y << std::endl;
		std::cout << "z: " << meshPlayer.positionInScene.z << std::endl;

		std::cout << "camera" << std::endl;
		std::cout << "x: " << vCamera.x << std::endl;
		std::cout << "y: " << vCamera.y << std::endl;
		std::cout << "z: " << vCamera.z << std::endl;
	}

	//!!TEMP!!
	Engine_Instantiate(meshCube);
	Engine_Instantiate(meshTest);

	if (Engine_MouseOver())
		std::cout << "merge!" << std::endl;
	else
		std::cout << "nu merge!" << std::endl;

	for (int i = 0; i < vecMeshesInScene.size(); i++)
	{
		bool a = Engine_OverlapGO(meshPlayer, vecMeshesInScene[i]);

		if (GetKey(olc::Key::O).bPressed)
		{
			std::cout << "overlapping: " << a << std::endl;
		}
	}

	//Clear the Screen
	Clear(olc::BLACK);

	//Clear Depth Buffer
	for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
		pDepthBuffer[i] = 0.0f;

	//Draw all GameObjects
	Engine_DrawGO();

	//Clear stored Triagles
	vecTrianglesToRaster.clear();

	//Clear stored Game Objects
	vecMeshesInScene.clear();

	//Clear view matrix from camera
	matView = { 0 };

	return true;
}

int main()
{
	//256, 240, 3, 3
	//1280, 720, 3, 3
	HatzEngine engine;
	if (engine.Construct(1280, 720, 3, 3, true))
		engine.Start();

	return 0;
}