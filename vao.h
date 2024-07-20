#pragma once

#include "vbo.h"

class VAO : public Bindable {
public:
	// Constructor that generates a Elements Buffer Object and links it to indices
	VAO()
	{
		glGenVertexArrays(1, &_id);
	}

	GLuint GetID() const
	{
		return _id;
	}

	void LinkAttribute(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	void Bind()
	{
		glBindVertexArray(_id);
	}

	// Unbinds the VAO
	void Unbind()
	{
		glBindVertexArray(0);
	}

	void Delete() override
	{
		glDeleteVertexArrays(1, &_id);
	}

private:
	GLuint _id;
};