#pragma once

#include "bindable.h"

class VBO : public Bindable {
public:
	VBO()
	{
		glGenBuffers(1, &_id);
	}

	void SetVertices(GLfloat* vertices, GLsizeiptr size, GLuint draw = GL_STATIC_DRAW)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, draw);
	}

	GLuint GetID() const
	{
		return _id;
	}

	void Bind() const override
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void Unbind() const override
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() override
	{
		glDeleteBuffers(1, &_id);
	}

private:
	GLuint _id;
};