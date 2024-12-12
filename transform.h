#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
public:
	Transform() : m_angle{}, m_position { 0 }, m_rotation{ 0 }, m_scale{ 1 }, m_model{} {}

	void Translate(glm::fvec3 translation)
	{
		m_position = translation;
		m_updated = false;
	}

	void Rotate(glm::vec3 rotation)
	{
		SetRotationArgs(rotation);
		m_updated = false;
	}

	void Scale(glm::vec3 scale)
	{
		m_scale = scale;
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

	glm::mat3 GetTransposeModelMatrix()
	{
		return glm::mat3(glm::transpose(glm::inverse(GetModelMatrix())));
	}

private:
	void UpdateModel()
	{

		m_model = glm::translate(glm::mat4(1.0f), m_position);

		if (m_rotation.x || m_rotation.y || m_rotation.z)
			m_model = glm::rotate(m_model, glm::radians(m_angle), m_rotation);

		m_model = glm::scale(m_model, m_scale);

		m_updated = true;
	}

	void SetRotationArgs(glm::fvec3& rotation)
	{
		rotation.x = normalizeAngle(rotation.x);
		rotation.y = normalizeAngle(rotation.y);
		rotation.z = normalizeAngle(rotation.z);

		m_angle = glm::length(rotation);
		if (rotation.x || rotation.y || rotation.z)
			m_rotation = glm::normalize(rotation);
	}

	static double normalizeAngle(double angle)
	{
		// Use the modulus operator to wrap the angle
		double normalizedAngle = fmod(angle, 360.0);

		// If the result is negative, adjust it to be in the range [0, 360)
		while (normalizedAngle < 0)
		{
			normalizedAngle += 360.0;
		}

		return normalizedAngle;
	}


private:
	bool m_updated = false;
	float m_angle;

	glm::fvec3 m_position;
	glm::fvec3 m_rotation;
	glm::fvec3 m_scale;

	glm::mat4 m_model;
};