#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

namespace
{
    inline void Push(std::vector<GLfloat>& data, glm::vec2 val)
    {
        data.push_back(val.x);
        data.push_back(val.y);
    }

    inline void Push(std::vector<GLfloat>& data, glm::vec3 val)
    {
        Push(data, glm::vec2(val));
        data.push_back(val.z);
    }

    inline void Push(std::vector<GLuint>& indices, const std::tuple<FaceIndex, FaceIndex, FaceIndex>& face, int FaceIndex::* index)
    {
        auto& [first, second, third] = face;
        indices.push_back(first.*index);
        indices.push_back(second.*index);
        indices.push_back(third.*index);
    }
}

namespace Utils
{
    inline Mesh WithoutEBO(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texture_coords,
        const std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces,
        const std::vector<BufferDataType>& types
    )
    {
        std::vector<GLfloat> data;

        for (auto& face : faces)
        {
            // Triangle
            auto& [index1, index2, index3] = face;

            for (auto& index : { index1, index2, index3 })
            {
                for (auto type : types)
                {
                    switch (type)
                    {
                    case BufferDataType::Vertex:
                        Push(data, vertices[index.vertexIndex]);
                        break;
                    case BufferDataType::Normal:
                        Push(data, normals[index.normalIndex]);
                        break;
                    case BufferDataType::TextureCoord:
                        Push(data, texture_coords[index.texCoordIndex]);
                        break;
                    default:
                        throw std::logic_error("Unknown BufferDataType");
                    }
                }
            }
        }

        return Mesh{ data, types, faces.size() };
    }

    inline Mesh WithEBO(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texture_coords,
        const std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces,
        const std::vector<BufferDataType>& types)
    {
        std::vector<GLfloat> data;
        std::vector<GLuint> indices;

        for (int i = 0; i < vertices.size(); i++) // Vertices size was chosen. May change this later
        {
            for (auto type : types)
            {
                switch (type)
                {
                case BufferDataType::Vertex:
                    Push(data, vertices[i]);
                    break;
                case BufferDataType::Normal:
                    Push(data, normals[i]);
                    break;
                case BufferDataType::TextureCoord:
                    Push(data, texture_coords[i]);
                    break;
                default:
                    throw std::logic_error("Unknown BufferDataType");
                }
            }
        }

        for (auto& face : faces)
        {
            Push(indices, face, &FaceIndex::vertexIndex);
        }


        return Mesh{ data, indices, types, faces.size() };
    }
}