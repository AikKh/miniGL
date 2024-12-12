#pragma once

#include "utils.h"

class Loader {
public:

    Mesh Load(const std::string& filename, std::vector<BufferDataType> types, bool ebo_use)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texture_coords;
        std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>> faces;

        LoadOBJ(filename, vertices, normals, texture_coords, faces);

        if (ebo_use)
        {
            return Utils::WithEBO(vertices, normals, texture_coords, faces, types);
        }
        return Utils::WithoutEBO(vertices, normals, texture_coords, faces, types);
    }

private: 
    void LoadOBJ(
        const std::string& filename, 
        std::vector<glm::vec3>& vertices, 
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texture_coords,
        std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces)
    {

        std::ifstream file{ filename };
        if (!file.is_open())
        {
            throw std::runtime_error("Error: Could not open file ");
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")
            {
                glm::vec3 vertex{};
                ss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (prefix == "vt")
            {
                glm::vec2 texCoord{};
                ss >> texCoord.x >> texCoord.y;
                texture_coords.push_back(texCoord);
            }
            else if (prefix == "vn")
            {
                glm::vec3 normal{};
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "f")
            {
                std::vector<FaceIndex> face;
                std::string vertexData;

                while (ss >> vertexData)
                {
                    std::istringstream vertexSS(vertexData);
                    std::string token;
                    FaceIndex faceIndex = { 0, 0, 0 };

                    std::getline(vertexSS, token, '/');
                    faceIndex.vertexIndex = std::stoi(token) - 1;

                    if (std::getline(vertexSS, token, '/'))
                    {
                        if (!token.empty())
                        {
                            faceIndex.texCoordIndex = std::stoi(token) - 1;
                        }
                        if (std::getline(vertexSS, token, '/'))
                        {
                            if (!token.empty())
                            {
                                faceIndex.normalIndex = std::stoi(token) - 1;
                            }
                        }
                    }

                    face.push_back(faceIndex);
                }

                if (face.size() < 3 || face.size() > 4)
                {
                    throw std::logic_error("Polygon should have 3 or 4 vertices");
                }

                faces.push_back(std::make_tuple(face[2], face[1], face[0]));

                if (face.size() == 4)
                {
                    faces.push_back(std::make_tuple(face[3], face[2], face[0]));
                }
            }
        }
        file.close();
    }
};