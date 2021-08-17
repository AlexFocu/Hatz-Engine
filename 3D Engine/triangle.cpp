#include "triangle.h"

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
{
	//Make sure plane normal is indeed normal
	plane_n = Vector_Normalise(plane_n);

	//Return signed shortest distance from point to plane
	auto dist = [&](vec3d& p)
	{
		vec3d n = Vector_Normalise(p);

		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
	};

	//Create two temporary storage arrays to classify points either side of plane
	//If distance sign is positive, point lies on "inside" of plane 
	vec3d* inside_points[3];  int nInsidePointCount = 0;
	vec3d* outside_points[3]; int nOutsidePointCount = 0;

	vec2d* inside_tex[3]; int nInsideTexCount = 0;
	vec2d* outside_tex[3]; int nOutsideTexCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);

	//Check to see if the point is in inside or outside the plane
	if (d0 >= 0) { 
		inside_points[nInsidePointCount++] = &in_tri.p[0]; inside_tex[nInsideTexCount++] = &in_tri.t[0]; 
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p[0]; outside_tex[nOutsideTexCount++] = &in_tri.t[0];
	}

	if (d1 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p[1]; inside_tex[nInsideTexCount++] = &in_tri.t[1];
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p[1];  outside_tex[nOutsideTexCount++] = &in_tri.t[1];
	}

	if (d2 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p[2]; inside_tex[nInsideTexCount++] = &in_tri.t[2];
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p[2];  outside_tex[nOutsideTexCount++] = &in_tri.t[2];
	}

	//classify triangle points, and break the input triangle into 
	//smaller output triangles if required
	switch (nInsidePointCount)
	{
	case 0:
		//All points lie on the outside of plane, so clip whole triangle

		//No returned triangles are valid
		return 0;

	case 3:
		//All points lie on the inside of plane, so do nothing 
		//and allow the triangle to simply pass through
		out_tri1 = in_tri;

		//Just the one returned original triangle is valid
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		//Triangle should be clipped. As two points lie outside
		//the plane, the triangle simply becomes a smaller triangle

		//Copy appearance info to new triangle
		out_tri1.col = in_tri.col;

		//The inside point is valid
		out_tri1.p[0] = *inside_points[0];
		out_tri1.t[0] = *inside_tex[0];

		//but the two new points are at the locations where the 
		//original sides of the triangle (lines) intersect with the plane
		float t;
		out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);

		out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);

		out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

		//Return the newly formed single triangle
		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		//Triangle should be clipped. As two points lie inside the plane,
		//the clipped triangle becomes a "quad". Fortunately, we can
		//represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.col = in_tri.col;

		out_tri2.col = in_tri.col;

		//The first triangle consists of the two inside points and a new
		//point determined by the location where one side of the triangle
		//intersects with the plane
		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];

		out_tri1.t[0] = *inside_tex[0];
		out_tri1.t[1] = *inside_tex[1];

		float t;
		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);

		out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		//The second triangle is composed of one of he inside points, a
		//new point determined by the intersection of the other side of the 
		//triangle and the plane, and the newly created point above
		out_tri2.p[0] = *inside_points[1];
		out_tri2.t[0] = *inside_tex[1];

		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.t[1] = out_tri1.t[2];

		out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);

		out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
		out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
		out_tri2.t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;

		//Return two newly formed triangles which form a quad
		return 2;
	}
}

float Triangle_DistanceToAPoint(vec3d& plane_n, vec3d& plane_p, vec3d& p)
{
	vec3d v = Vector_Sub(p, plane_p);	
	return Vector_DotProduct(v, plane_n);
}

float Triangle_Area(triangle& tri)
{
	float area = ( tri.p[0].x * (tri.p[1].y - tri.p[2].y) +
			  	   tri.p[1].x * (tri.p[2].y - tri.p[0].y) +
				   tri.p[2].x * (tri.p[0].y - tri.p[3].y) ) / 2;

	return area;
}

bool Triangle_InsidePoint(triangle& tri, olc::vi2d& cam)
{
	float startArea = Triangle_Area(tri);

	vec3d vCam = { cam.x, cam.y, 0 };

	std::vector<triangle> triList;

	for (int i = 0; i < 3; i++)
	{
		//Get a edge 
		vec3d edge = Vector_Sub(tri.p[i], tri.p[(i + 1) % 3]);

		triangle a = { tri.p[i], tri.p[(i + 1) % 3], vCam};

		triList.push_back(a);
	}

	float finalArea = 0;
	for (triangle& tris : triList)
	{
		finalArea += Triangle_Area(tris);
	}

	if (startArea == finalArea)
		return true;
	else
		return false;

}