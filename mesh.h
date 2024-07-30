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
	Mesh(const std::vector<glm::vec3>& vertices, 
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces) : FaceCount{ faces.size() }
	{
		GenerateVAO(vertices, normals, texCoords, faces);
	}

	void Activate() const
	{
		m_vao.Bind();
	}

private:
	void GenerateVAO(const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces)
	{
		std::vector<GLfloat> v;

		for (auto& face : faces)
		{
			auto& [index1, index2, index3] = face;

			for (auto& index : { index1, index2, index3 })
			{
				v.push_back(vertices[index.vertexIndex].x); 
				v.push_back(vertices[index.vertexIndex].y);
				v.push_back(vertices[index.vertexIndex].z);

				v.push_back(normals[index.normalIndex].x);
				v.push_back(normals[index.normalIndex].y);
				v.push_back(normals[index.normalIndex].z);

				v.push_back(texCoords[index.texCoordIndex].x);
				v.push_back(texCoords[index.texCoordIndex].y);
			}
		}

		//EBO ebo{ indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLfloat)) };

		m_vao.Bind();
		m_vbo.SetVertices(v.data(), static_cast<GLsizeiptr>(v.size() * sizeof(GLfloat)));

		if (!ensureBoundedVBO())
		{
			return;
		}

		m_vao.LinkAttribute(m_vbo, 0, 3, GL_FLOAT, 8* sizeof(float), (void*)0); // vertices
		
		m_vao.LinkAttribute(m_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normals
		m_vao.LinkAttribute(m_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texCoords

		m_vbo.Unbind();
		m_vao.Unbind();

		// Check for OpenGL errors
		checkOpenGLErrors();
	}

	static void checkOpenGLErrors()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error after linking attributes: " << error << std::endl;
		}
	}

	static bool ensureBoundedVBO()
	{
		GLint vboBinding;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vboBinding);
		if (vboBinding == 0)
		{
			std::cerr << "VBO is not bound correctly." << std::endl;
			return false;
		}

		return true;
	}

public:
	const std::size_t FaceCount;

private:
	VAO m_vao;
	VBO m_vbo;
	//EBO m_ebo;
};


