#pragma once

#include "bindable.h"

class VBO : public Bindable {
public:
	VBO(GLfloat* vertices, GLsizeiptr size, GLuint draw = GL_STATIC_DRAW)
	{
		glGenBuffers(1, &_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, draw);
	}

	GLuint GetID() const
	{
		return _id;
	}

	void Bind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void Unbind() override
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