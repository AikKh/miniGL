#pragma once

#include <functional>

#include "transform.h"
#include "mesh.h"
#include "renderer.h"

class Object {
public:
	Object(std::shared_ptr<Transform> transform, std::shared_ptr<Mesh> mesh, std::shared_ptr<Renderer> renderer, std::function<void(const Object&)> updater) :
		Transform{ std::move(transform) }, Mesh{ std::move(mesh) }, Renderer{ std::move(renderer) }, m_updater{ std::move(updater) }
	{
	}

	void Select() const
	{
		Mesh->Activate();
		Renderer->Activate();
	}

	void CallUpdater() const
	{
		m_updater(*this);
	}

public:
	std::shared_ptr<Transform> Transform;
	std::shared_ptr<Mesh> Mesh;
	std::shared_ptr<Renderer> Renderer;

private:
	std::function<void(const Object&)> m_updater;
};
