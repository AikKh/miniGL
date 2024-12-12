#pragma once

#include "object.h"

class Scene {
public:
	using iterator = std::vector<Object>::iterator;
	using const_iterator = std::vector<Object>::const_iterator;

	iterator begin() { return m_objects.begin(); }
	iterator end() { return m_objects.end(); }

	const_iterator begin() const { return m_objects.cbegin(); }
	const_iterator end() const { return m_objects.cend(); }

	void Add(Object object)
	{
		m_objects.push_back(std::move(object));
	}

private:
	std::vector<Object> m_objects;
};