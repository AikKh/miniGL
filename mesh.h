#pragma once

#include "includes.h"

#include "ebo.h"
#include "vbo.h"
#include "vao.h"

struct FaceIndex {
	int vertexIndex, texCoordIndex, normalIndex;
};

class Mesh {
public:
	Mesh()
	{

	}

	VAO GenerateVAO()
	{
		// TODO: Add EBO
		std::vector<GLfloat> vertices;
		std::vector<GLuint> indices;

		for (auto& face : _faces)
		{
			auto& [index1, index2, index3] = face;

			for (auto& index : { index1, index2, index3 })
			{
				vertices.push_back(m_vertices[index.vertexIndex].x);
				vertices.push_back(m_vertices[index.vertexIndex].y);
				vertices.push_back(m_vertices[index.vertexIndex].z);

				vertices.push_back(m_normals[index.normalIndex].x);
				vertices.push_back(m_normals[index.normalIndex].y);
				vertices.push_back(m_normals[index.normalIndex].z);

				vertices.push_back(m_texCoords[index.texCoordIndex].x);
				vertices.push_back(m_texCoords[index.texCoordIndex].y);

				//indices.push_back(index.vertexIndex);
			}
		}

		VBO vbo{ vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)) };
		//EBO ebo{ indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLfloat)) };


		VAO vao;
		vao.Bind();

		vbo.Bind();
		//ebo.Bind();

		// Ensure the VBO is bound correctly
		GLint vboBinding;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vboBinding);
		if (vboBinding == 0)
		{
			std::cerr << "VBO is not bound correctly." << std::endl;
			return vao;
		}

		vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		// Check for OpenGL errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error after linking attributes: " << error << std::endl;
		}

		return vao;
	}

	void AddVertex(const glm::vec3& point)
	{
		m_vertices.push_back(point);
	}

	void AddTextureCoord(const glm::vec2& point)
	{
		m_texCoords.push_back(point);
	}

	void AddNormal(const glm::vec3& point)
	{
		m_normals.push_back(point);
	}

	void AddFace(const FaceIndex& index1, const FaceIndex& index2, const FaceIndex& index3)
	{
		_faces.push_back(std::make_tuple(index1, index2, index3));
	}

	inline int GetIndicesCount() const
	{
		return _faces.size() * 3;
	}

	void Print() const
	{
		// Print vertices
		std::cout << "# Vertices" << std::endl;
		for (const auto& vertex : m_vertices)
		{
			std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
		}

		// Print texture coordinates
		std::cout << "# Texture Coordinates" << std::endl;
		for (const auto& texCoord : m_texCoords)
		{
			std::cout << "vt " << texCoord.x << " " << texCoord.y << std::endl;
		}

		// Print normals
		std::cout << "# Normals" << std::endl;
		for (const auto& normal : m_normals)
		{
			std::cout << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
		}

		// Print faces
		std::cout << "# Faces" << std::endl;
		for (const auto& face : _faces)
		{
			auto& [index1, index2, index3] = face;
			std::cout << "f "
				<< index1.vertexIndex + 1 << "/" << index1.texCoordIndex + 1 << "/" << index1.normalIndex + 1 << " "
				<< index2.vertexIndex + 1 << "/" << index2.texCoordIndex + 1 << "/" << index2.normalIndex + 1 << " "
				<< index3.vertexIndex + 1 << "/" << index3.texCoordIndex + 1 << "/" << index3.normalIndex + 1
				<< std::endl;
		}
	}

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_texCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>> _faces;

};