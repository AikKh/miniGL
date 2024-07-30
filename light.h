#pragma once

#include <glm/glm.hpp>

class Light {
public:
	Light(const glm::vec3& position) : m_position{ position } {}

	const glm::vec3& GetPosition() const
	{
		return m_position;
	}

private:
	glm::vec3 m_position;
};