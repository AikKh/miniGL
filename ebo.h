#pragma once

#include "bindable.h"

class EBO : public Bindable {
public:
	EBO()
	{
		glGenBuffers(1, &_id);
	}

	void SetIndices(GLuint* indices, GLsizeiptr size, GLuint draw = GL_STATIC_DRAW)
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, draw);
	}

	GLuint GetID() const
	{
		return _id;
	}

	void Bind() const override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void Unbind() const override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Delete() override
	{
		glDeleteBuffers(1, &_id);
	}

private:
	GLuint _id;
};