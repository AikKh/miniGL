#pragma once

#include "includes.h"

class Transform {
public:
	Transform() : m_position{ 0 }, m_rotation{ 0 }, m_scale{ 1 } {}

	void Translate(glm::fvec3 translation)
	{
		m_position += translation;
		m_updated = false;
	}

	void Rotate(glm::vec3 rotation)
	{
		m_rotation += rotation;
		m_updated = false;
	}

	void Scale(glm::vec3 scale)
	{
		m_scale += scale;
		m_updated = false;
	}
	
	const glm::mat4& GetModelMatrix() &
	{
		if (!m_updated)
		{
			UpdateModel();
		}
		return m_model;
	}

private:
	void UpdateModel()
	{
		float angle{};
		glm::fvec3 rotation{};
		GetRotationArgs(angle, rotation);


		m_model = glm::translate(glm::mat4(1.0f), m_position);
		m_model = glm::rotate(m_model, glm::radians(angle), rotation);
		m_model = glm::scale(m_model, m_scale);
	}

	void GetRotationArgs(float& angle, glm::fvec3 rotation)
	{
		m_rotation.x %= 360;
		m_rotation.y %= 360;
		m_rotation.z %= 360;

		angle = glm::length(m_rotation);
		rotation = glm::normalize(m_rotation);
	}

private:
	bool m_updated = false;

	glm::fvec3 m_position;
	glm::ivec3 m_rotation;
	glm::fvec3 m_scale;

	glm::mat4 m_model;
};