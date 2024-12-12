#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Bindable {
public:
	virtual void Bind() const = 0;
	
	virtual void Unbind() const = 0;
	
	virtual void Delete() = 0;
};
