#pragma once

#include "utils.h"
#include "perlin_noise.h"

#include <glm/gtc/noise.hpp>

class Builder
{
public:
	Mesh Cube() const
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>> faces;

		// Vertices
		for (int i = -1; i <= 1; i += 2)
		{
			for (int j = -1; j <= 1; j += 2)
			{
				for (int k = -1; k <= 1; k += 2)
				{
					vertices.push_back({ i, j, k });
				}
			}
		}

		// Normals
		normals.emplace_back(0, 0, -1);
		normals.emplace_back(0, 0, 1);
		normals.emplace_back(0, -1, 0);
		normals.emplace_back(0, 1, 0);
		normals.emplace_back(-1, 0, 0);
		normals.emplace_back(1, 0, 0);

		// Left
		faces.push_back({ { 0, 0, 4 }, { 2, 0, 4 }, { 3, 0, 4 } });
		faces.push_back({ { 3, 0, 4 }, { 1, 0, 4 }, { 0, 0, 4 } });

		// Right
		faces.push_back({ { 4, 0, 5 }, { 5, 0, 5 }, { 7, 0, 5 } });
		faces.push_back({ { 7, 0, 5 }, { 6, 0, 5 }, { 4, 0, 5 } });

		// Up
		faces.push_back({ { 3, 0, 3 }, { 2, 0, 3 }, { 6, 0, 3 } });
		faces.push_back({ { 6, 0, 3 }, { 7, 0, 3 }, { 3, 0, 3 } });

		// Down
		faces.push_back({ { 0, 0, 2 }, { 1, 0, 2 }, { 5, 0, 2 } });
		faces.push_back({ { 5, 0, 2 }, { 4, 0, 2 }, { 0, 0, 2 } });

		// Front
		faces.push_back({ { 1, 0, 1 }, { 3, 0, 1 }, { 7, 0, 1 } });
		faces.push_back({ { 7, 0, 1 }, { 5, 0, 1 }, { 1, 0, 1 } });

		// Back
		faces.push_back({ { 6, 0, 0 }, { 2, 0, 0 }, { 0, 0, 0 } });
		faces.push_back({ { 0, 0, 0 }, { 4, 0, 0 }, { 6, 0, 0 } });

		// Order of vertices is very important, because of culling face optimization.

		return Utils::WithoutEBO(vertices, normals, {}, faces, { BufferDataType::Vertex, BufferDataType::Normal });
	}

	Mesh Plane(float x, float y, float z, float size) const
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>> faces;

		for (int i = -1; i <= 1; i += 2)
		{
			for (int j = -1; j <= 1; j += 2)
			{
				vertices.push_back({ (i * size) + x, y, (j * size) + z });
			}
		}

		normals.emplace_back(0, 1, 0);
		
		faces.push_back({ { 1, 0, 0 }, { 0, 0, 0 }, { 2, 0, 0 } });
		faces.push_back({ { 2, 0, 0 }, { 3, 0, 0 }, { 1, 0, 0 } });

		return Utils::WithoutEBO(vertices, normals, {}, faces, { BufferDataType::Vertex, BufferDataType::Normal });
	}

	Mesh Noise(int width, int height) const
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>> faces;

		// Generate perlin noise here
		PerlinNoise noise;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				//vertices.emplace_back(x, glm::perlin(glm::vec2{ (float)x / width, (float)y / height }) * 10, y);
				auto n = noise.Noise(float(x) / width, float(y) / height) * 25;
				vertices.emplace_back(x, n, y);
				//std::cout << n << " ";
			}
			//std::cout << std::endl;
		}

		// Calculate the nomal of triangles using cross product
		for (int y = 0; y < height - 1; y++)
		{
			for (int x = 0; x < width - 1; x++)
			{
				auto tl = vertices[y * width + x];
				auto bl = vertices[(y+1) * width + x];
				auto tr = vertices[y * width + x + 1];
				auto br = vertices[(y+1) * width + x + 1];

				//std::cout << y * 10 + x << ", " << (y+1) * 10 + x << ", " << y * 10 + x + 1 << ", " << (y + 1) * 10 + x + 1 << ", " << std::endl;

				normals.push_back(glm::normalize(glm::cross(tl - tr, br - tr)));
				normals.push_back(glm::normalize(-glm::cross(tl - bl, br - bl)));
			}
		}

		// Index all this using faces, clockwise!
		for (int y = 0; y < height - 1; y++)
		{
			for (int x = 0; x < width - 1; x++)
			{
				int tl = y * width + x;
				int bl = (y+1) * width + x;
				int tr = y * width + x + 1;
				int br = (y + 1) * width + x + 1;

				int n1 = (y * (width-1) + x) * 2;
				int n2 = (y * (width-1) + x) * 2 + 1;

				faces.push_back({ { tl, 0, n1 }, { tr, 0, n1 }, { br, 0, n1 } });
				faces.push_back({ { br, 0, n2 }, { bl, 0, n2 }, { tl, 0, n2 } });
			}
		}

		return Utils::WithoutEBO(vertices, normals, {}, faces, { BufferDataType::Vertex, BufferDataType::Normal });
	}

private:
};