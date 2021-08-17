#include "HatzEngine.h"

HatzEngine::HatzEngine()
{
	sAppName = "No Name";
}

bool HatzEngine::Engine_MouseOver()
{
	olc::vi2d a = GetMousePos();
	for (triangle t : vecTrianglesToRaster)
	{
		if (Triangle_InsidePoint(t, a))
			return true;
	}

	return false;
}

void HatzEngine::Player_Move(float fRotationSpeed, float fJumpHeight)
{
	//Uncomment if you want the Player to be affected by gravity
	//Player_ApplyGravity();

	//!!Temp!!
	if (GetKey(olc::Key::UP).bHeld)
		vCamera.y += fSpeed * GetElapsedTime();

	if (GetKey(olc::Key::DOWN).bHeld)
		vCamera.y -= fSpeed * GetElapsedTime();

	vec3d vForward = Vector_Mul(vLookDir, fSpeed * GetElapsedTime());

	if (GetKey(olc::Key::W).bHeld)
		vCamera = Vector_Add(vCamera, vForward);

	if (GetKey(olc::Key::S).bHeld)
		vCamera = Vector_Sub(vCamera, vForward);

	if (GetKey(olc::Key::D).bHeld)
		fYaw += fRotationSpeed * GetElapsedTime();

	if (GetKey(olc::Key::A).bHeld)
		fYaw -= fRotationSpeed * GetElapsedTime();

	if (GetKey(olc::Key::SPACE).bPressed && bIsGrounded)
	{
		vVelocity.y = sqrtf(fJumpHeight * -2.0f * fGravity);
		bIsGrounded = false; //temp
	}
}

void HatzEngine::Player_ApplyGravity()
{
	if(bIsGrounded && vVelocity.y < 0)
		vVelocity.y = -2.0f;

	vVelocity.y += fGravity * GetElapsedTime();

	vec3d g = Vector_Mul(vVelocity, GetElapsedTime());
	vCamera = Vector_Add(vCamera, g);

	bIsGrounded = true; //temp
}

void HatzEngine::Engine_WaitForSeconds(float fTime)
{
	while (fTime > GetElapsedTime()) {}
}

void HatzEngine::Engine_SetPosition(mesh& m, float x, float y, float z)
{
	m.positionInScene.x = x;
	m.positionInScene.y = y;
	m.positionInScene.z = z;
}

void HatzEngine::TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
								  int x2, int y2, float u2, float v2, float w2,
								  int x3, int y3, float u3, float v3, float w3,
								  olc::Sprite* tex)
{
	//Sort variables based on the Y position on the screen
	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
		std::swap(w1, w2);
	}

	if (y3 < y1)
	{
		std::swap(y1, y3);
		std::swap(x1, x3);
		std::swap(u1, u3);
		std::swap(v1, v3);
		std::swap(w1, w3);
	}

	if (y3 < y2)
	{
		std::swap(y2, y3);
		std::swap(x2, x3);
		std::swap(u2, u3);
		std::swap(v2, v3);
		std::swap(w2, w3);
	}

	//First line of the Triangle
	int dy1 = y2 - y1;
	int dx1 = x2 - x1;

	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	//Second line of the Triangle 
	int dy2 = y3 - y1;
	int dx2 = x3 - x1;

	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	//The final points of the Texture
	float tex_u, tex_v, tex_w;

	//How many pixels it should move on X
	float dax_step = 0, dbx_step = 0,
		  du1_step = 0, dv1_step = 0,
		  du2_step = 0, dv2_step = 0,
		  dw1_step = 0, dw2_step = 0;

	//If the line between the 2 points in horizontal is infinity
	if (dy1)
	{
		dax_step = dx1 / (float)abs(dy1);

		du1_step = du1 / (float)abs(dy1);
		dv1_step = dv1 / (float)abs(dy1);
		dw1_step = dw1 / (float)abs(dy1);
	}

	if (dy2)
	{
		dbx_step = dx2 / (float)abs(dy2);
		
		du2_step = du2 / (float)abs(dy2);
		dv2_step = dv2 / (float)abs(dy2);	
		dw2_step = dw2 / (float)abs(dy2);
	}

	//Draw Top Traingle
	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			//Vertex position along the Edge
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			//Starting points
			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			//Ending points
			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			//Sort along the X-axis to ensure it is drawing from Start -> End
			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			//Set the Starting location 
			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			//Draw the Traingle
			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
				{
					Draw(j, i, tex->Sample(tex_u / tex_w, tex_v / tex_w));

					pDepthBuffer[i * ScreenWidth() + j] = tex_w;
				}

				t += tstep;
			}
		}
	}

	//Update gradients
	dy1 = y3 - y2;
	dx1 = x3 - x2;

	dv1 = v3 - v2;
	du1 = u3 - u2;

	dw1 = w3 - w2;

	//If the line between the 2 points in horizontal is infinity
	du1_step = 0, dv1_step = 0; 
	if (dy1)
	{
		dax_step = dx1 / (float)abs(dy1);

		du1_step = du1 / (float)abs(dy1);
		dv1_step = dv1 / (float)abs(dy1);
		dw1_step = dw1 / (float)abs(dy1);
	}

	if (dy2)
	{
		dbx_step = dx2 / (float)abs(dy2);
	}

	//Draw the bottom Traingle
	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			//Vertex position along the Edge
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			//Starting points
			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			//Ending points
			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			//Sort along the X-axis to ensure it is drawing from Start -> End
			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			//Set the Starting location 
			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			//Draw the Traingle
			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
				{
					Draw(j, i, tex->Sample(tex_u / tex_w, tex_v / tex_w));

					pDepthBuffer[i * ScreenWidth() + j] = tex_w;
				}

				t += tstep;
			}
		}
	}
}

void HatzEngine::Engine_Instantiate(mesh& meshInst)
{
	//Add to List of Game Objects in Scene
	vecMeshesInScene.push_back(meshInst);

	//World Matrix
	mat4x4 matWorld = Matrix_WorldMatrix(meshInst);

	for (triangle tri : meshInst.tris)
	{
		triangle triProjected, triTransformed, triViewed;

		//World Matrix Transform
		triTransformed = Matrix_Transform(matWorld, tri);

		//Carry on the texture coordonates of the Triangle
		triTransformed.t[0] = tri.t[0];
		triTransformed.t[1] = tri.t[1];
		triTransformed.t[2] = tri.t[2];

		//Calculate triangle Normal
		vec3d normal = Vector_Normal(triTransformed.p[0], triTransformed.p[1], triTransformed.p[2]);

		//Get Ray from the triangle to the camera
		vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera); 

		//If ray is aligned with normal, then the triangle is visible
		if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
		{
			//Directional Light
			vec3d light_direction = { 0.0f, 0.0f, -1.0f };

			//Normalize the Directional Light
			light_direction = Vector_Normalise(light_direction);

			//How "aligned are the light direction and the triangle surface normal?
			float dp = std::max(0.1f, Vector_DotProduct(light_direction, normal));

			//Color the Triangle
			triTransformed.col = olc::Pixel(dp * 255, dp * 255, dp * 255);

			//Convert World Space -> View Space
			triViewed = Matrix_Transform(matView, triTransformed);

			//Carry on the color of the Triangle
			triViewed.col = triTransformed.col;

			//Carry on the texture coordonates of the Triangle
			triViewed.t[0] = triTransformed.t[0];
			triViewed.t[1] = triTransformed.t[1];
			triViewed.t[2] = triTransformed.t[2];

			//Clip Viewed Triangle against near plane
			int nClippedTriangles = 0;
			triangle clipped[2];
			nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

			for (int n = 0; n < nClippedTriangles; n++)
			{
				//Project the triangle from 3D -> 2D
				triProjected = Matrix_Transform(matProj, clipped[n]);

				//Carry on the color of the Triangle
				triProjected.col = clipped[n].col;

				//Carry on the texture coordonates of the Triangle
				triProjected.t[0] = clipped[n].t[0];
				triProjected.t[1] = clipped[n].t[1];
				triProjected.t[2] = clipped[n].t[2];
				
				//Modify texture coordonates
				triProjected.t[0].u /= triProjected.p[0].w;
				triProjected.t[1].u /= triProjected.p[1].w;
				triProjected.t[2].u /= triProjected.p[2].w;

				triProjected.t[0].v /= triProjected.p[0].w;
				triProjected.t[1].v /= triProjected.p[1].w;
				triProjected.t[2].v /= triProjected.p[2].w;

				triProjected.t[0].w = 1.0f / triProjected.p[0].w;
				triProjected.t[1].w = 1.0f / triProjected.p[1].w;
				triProjected.t[2].w = 1.0f / triProjected.p[2].w;

				//Scale into view the Triangle
				triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
				triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
				triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

				// X/Y are inverted so put them back
				triProjected.p[0].x *= -1.0f;
				triProjected.p[1].x *= -1.0f;
				triProjected.p[2].x *= -1.0f;
				triProjected.p[0].y *= -1.0f;
				triProjected.p[1].y *= -1.0f;
				triProjected.p[2].y *= -1.0f;

				//Offset verts into visible normalised space
				vec3d vOffsetView = { 1, 1, 0 };
				triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
				triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
				triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);

				triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

				//Save the texture to the Tringle
				triProjected.tex = meshInst.tex;

				//Store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}
		}
	}
}

bool HatzEngine::Engine_OverlapGO(mesh& mesh1, mesh& mesh2)
{
	mesh m1 = mesh1;
	mesh m2 = mesh2;

	//World Matrix
	mat4x4 matWorld1 = Matrix_WorldMatrix(m1);
	mat4x4 matWorld2 = Matrix_WorldMatrix(m2);

	//World Matrix Transform
	for (triangle& tri : m1.tris)
	{
		tri = Matrix_Transform(matWorld1, tri);
	}

	for (triangle& tri : m2.tris)
	{
		tri = Matrix_Transform(matWorld2, tri);
	}

	//Iterate thru the Triangles of the first Mesh 
	for (triangle& tri1 : m1.tris)
	{
		bool bAllOut = true;

		//Calculate the Normal of the plane
		vec3d normal = Vector_Normal(tri1.p[0], tri1.p[1], tri1.p[2]);

		//Iterate thru the Points of the second Mesh 
		for (triangle& tri2 : m2.tris)
		{
			for (vec3d& point : tri2.p)
			{
				if (Triangle_DistanceToAPoint(normal, tri1.p[0], point) < 0.0f)
				{
					bAllOut = false;
					break;
				}
			}
		}

		//A separating plane was found, do not continue testing
		if (bAllOut)
			return false;
	}

	//Iterate thru the Triangles of the second Mesh 
	for (triangle& tri2 : m2.tris)
	{
		bool bAllOut = true;

		//Calculate the Normal of the plane
		vec3d normal = Vector_Normal(tri2.p[0], tri2.p[1], tri2.p[2]);

		//Iterate thru the Points of the first Mesh 
		for (triangle& tri1 : m1.tris)
		{
			for (vec3d& point : tri1.p)
			{
				if (Triangle_DistanceToAPoint(normal, tri2.p[0], point) < 0.0f)
				{
					bAllOut = false;
					break;
				}
			}
		}

		//A separating plane was found, do not continue testing
		if (bAllOut)
			return false;
	}

	//Iterate thru the Edges of the first Mesh 
	for (triangle& tri1 : m1.tris)
	{
		for (int i = 0; i < 3; i++)
		{
			//Get a edge from the first Mesh 
			vec3d edge1 = Vector_Sub(tri1.p[i], tri1.p[(i + 1) % 3]);

			//Iterate thru the Edges of the second Mesh 
			for (triangle& tri2 : m2.tris)
			{	
				for (int j = 0; j < 3; j++)
				{
					//Get a edge from the second Mesh 
					vec3d edge2 = Vector_Sub(tri2.p[j], tri2.p[(j + 1) % 3]);

					vec3d normal = Vector_CrossProduct(edge1, edge2);

					if (Vector_DotProduct(normal, normal) == 0.0f)
						continue;

					float maxA = -INFINITY;
					float minA = INFINITY;

					//Iterate thru the Points of the first Mesh 
					for (triangle& tri : m1.tris)
					{
						for (vec3d& point : tri.p)
						{
							float distance = Vector_DotProduct(normal, point);

							maxA = std::max(maxA, distance);
							minA = std::min(minA, distance);
						}
					}

					float maxB = -INFINITY;
					float minB = INFINITY;

					//Iterate thru the Points of the second Mesh 
					for (triangle& tri : m2.tris)
					{
						for (vec3d& point : tri.p)
						{
							float distance = Vector_DotProduct(normal, point);

							maxB = std::max(maxB, distance);
							minB = std::min(minB, distance);
						}
					}

					if (minA > maxB || minB > maxA)
						return false;
				}
			}
		}
	}

	return true;
}

void HatzEngine::Engine_MoveCamera()
{
	//test
	meshPlayer.positionInScene.x = vCamera.x;
	meshPlayer.positionInScene.y = vCamera.y - 1.0f;
	meshPlayer.positionInScene.z = vCamera.z;

	// Create "Point At" Matrix for camera
	vec3d vUp = { 0, 1, 0 };
	vec3d vTarget = { 0, 0, 1 };

	//Roatate by Yaw (turns the Player from left to right)
	mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);

	//Roatate the Target by the Rotation Matrix
	vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);

	//Offset to the camera rotation
	vTarget = Vector_Add(vCamera, vLookDir);

	mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

	//Make view matrix from camera
	matView = Matrix_QuickInverse(matCamera);
}

void HatzEngine::Engine_DrawGO()
{
	//Draw Models
	for (auto& triToRaster : vecTrianglesToRaster)
	{
		//Clip triangles against all four screen edges, this could yield
		//a bunch of triangles, so create a queue that we traverse to 
		//ensure we only test new triangles generated against planes
		triangle clipped[2];
		std::list<triangle> listTriangles;

		//Add initial triangle
		listTriangles.push_back(triToRaster);

		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				//Take triangle from front of queue
				triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				//Clip it against a plane. We only need to test each 
				//subsequent plane, against subsequent new triangles
				//as all triangles after a plane clip are guaranteed
				//to lie on the inside of the plane
				switch (p)
				{
				case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				//Clipping may yield a variable number of triangles, so
				//add these new ones to the back of the queue for subsequent
				//clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++)
				{
					//Carry on the texture of the Triangle
					clipped[w].tex = triToRaster.tex;

					listTriangles.push_back(clipped[w]);
				}
			}

			nNewTriangles = listTriangles.size();
		}

		//Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& t : listTriangles)
		{
			TexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
							 t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
							 t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w,
							 t.tex);

			//FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.col);
			// !!FOR DEBUG!!
			//DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, olc::WHITE);
		}
	}
}