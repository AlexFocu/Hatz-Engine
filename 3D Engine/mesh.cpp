#include "mesh.h"

#include <fstream>
#include <sstream>

//mesh::mesh(std::string sTag, olc::Sprite* sprTex) : tag(sTag), tex(sprTex){}

//Load the Model
bool mesh::LoadFromObjectFile(std::string sFileName, bool bHasTexture)
{
	std::ifstream f(sFileName);
	if (!f.is_open())
		return false;

	//Local cache of verts
	std::vector<vec3d> verts;
	std::vector<vec2d> texs;

	while (!f.eof())
	{
		//The line limit of the Model needs to be under 128
		char line[128];
		f.getline(line, 128);

		//Read the line
		std::stringstream s;
		s << line;

		char junk;

		//If it is a vert
		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				vec2d v;
				s >> junk >> junk >> v.u >> v.v;

				v.u = 1.0f - v.u;
				v.v = 1.0f - v.v;

				texs.push_back(v);
			}
			else
			{
				//Read the vert info and put it in the local cache of verts
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}

		if (!bHasTexture)
		{
			//If it is a face
			if (line[0] == 'f')
			{
				//Read it and put it in the mesh.tris vector
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
		else
		{
			if (line[0] == 'f')
			{
				s >> junk;

				std::string tokens[6];
				int nTokenCount = -1;

				while (!s.eof())
				{
					char c = s.get();
					if (c == ' ' || c == '/')
						nTokenCount++;
					else
						tokens[nTokenCount].append(1, c);
				}

				tokens[nTokenCount].pop_back();

				tris.push_back({ verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
					texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] });

			}
		}
	}

	return true;
}
