#pragma once

#include "bindable.h"

class EBO : public Bindable {
public:
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	GLuint GetID() const
	{
		return _id;
	}

	void Bind() override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void Unbind() override
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