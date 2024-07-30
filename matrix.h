#pragma once

#include "includes.h"

class Matrix {
public:
	const glm::mat4& Get() const
	{
		if (!m_updated)
		{
			Update();
		}
		return m_matrix;
	}

	virtual void Update() const = 0;

private:
	bool m_updated = false;
	mutable glm::mat4 m_matrix;
};