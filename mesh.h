#pragma once

#include <iostream>
#include <numeric>
#include <vector>

#include "ebo.h"
#include "vbo.h"
#include "vao.h"

enum class BufferDataType {
	Vertex,
	Normal,
	TextureCoord,
};

struct FaceIndex {
	int vertexIndex, texCoordIndex, normalIndex;
};

class Mesh {
public:
	Mesh(std::vector<GLfloat> vertex_data, std::vector<BufferDataType> types, std::size_t face_count) : m_face_count{ face_count }, m_ebo_use { false }
	{
		m_vao.Bind();
		m_vbo.SetVertices(vertex_data.data(), static_cast<GLsizeiptr>(vertex_data.size() * sizeof(GLfloat)));

		SetUp(types);
	}

	Mesh(std::vector<GLfloat> vertex_data, std::vector<GLuint> indices, std::vector<BufferDataType> types, std::size_t face_count) : m_face_count{ face_count }, m_ebo_use{ true }
	{
		m_vao.Bind();
		m_vbo.SetVertices(vertex_data.data(), static_cast<GLsizeiptr>(vertex_data.size() * sizeof(GLfloat)));
		m_ebo.SetIndices(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLfloat)));

		SetUp(types);
	}

	void Activate() const
	{
		m_vao.Bind();
	}

	bool UsesEBO() const
	{
		return m_ebo_use;
	}

	int GetFaceCount() const
	{
		return m_face_count;
	}

private:
#pragma region OldCode

	//void GenerateVAO(const std::vector<glm::vec3>& vertices,
	//	const std::vector<glm::vec3>& normals,
	//	const std::vector<glm::vec2>& texCoords,
	//	const std::vector<std::tuple<FaceIndex, FaceIndex, FaceIndex>>& faces)
	//{
	//	std::vector<GLfloat> v;

	//	for (auto& face : faces)
	//	{
	//		auto& [index1, index2, index3] = face;

	//		for (auto& index : { index1, index2, index3 })
	//		{
	//			v.push_back(vertices[index.vertexIndex].x); 
	//			v.push_back(vertices[index.vertexIndex].y);
	//			v.push_back(vertices[index.vertexIndex].z);

	//			v.push_back(normals[index.normalIndex].x);
	//			v.push_back(normals[index.normalIndex].y);
	//			v.push_back(normals[index.normalIndex].z);

	//			v.push_back(texCoords[index.texCoordIndex].x);
	//			v.push_back(texCoords[index.texCoordIndex].y);
	//		}
	//	}

	//	//EBO ebo{ indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLfloat)) };

	//	m_vao.Bind();
	//	m_vbo.SetVertices(v.data(), static_cast<GLsizeiptr>(v.size() * sizeof(GLfloat)));

	//	if (!ensureBoundedVBO())
	//	{
	//		return;
	//	}

	//	m_vao.LinkAttribute(m_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // vertices
	//	
	//	m_vao.LinkAttribute(m_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normals
	//	m_vao.LinkAttribute(m_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texCoords

	//	m_vbo.Unbind();
	//	m_vao.Unbind();

	//	// Check for OpenGL errors
	//	checkOpenGLErrors();
	//}

#pragma endregion

	void SetUp(const std::vector<BufferDataType>& types)
	{
		auto format = getFormat(types);

		if (!ensureBoundedVBO())
		{
			return;
		}

		std::vector<int> accumulated(format.size());
		std::partial_sum(format.begin(), format.end(), accumulated.begin());


		for (std::size_t i = 0; i < format.size(); i++)
		{
			m_vao.LinkAttribute(m_vbo, i, format[i], GL_FLOAT,
				accumulated.back() * sizeof(float), (void*)((accumulated[i] - format[i]) * sizeof(float)));
		}

		m_vbo.Unbind();
		m_vao.Unbind();

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

	static std::vector<int> getFormat(const std::vector<BufferDataType>& types)
	{
		std::vector<int> format;
		for (auto type : types)
		{
			format.push_back(type == BufferDataType::TextureCoord?2:3); // Hardcooded for now
		}

		return format;
	}

private:
	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;

	const std::size_t m_face_count;
	const bool m_ebo_use;
};


