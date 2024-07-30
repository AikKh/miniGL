#pragma once

#include "transform.h"
#include "mesh.h"
#include "renderer.h"

struct Object {
	Object(std::shared_ptr<Transform> transform, std::shared_ptr<Mesh> mesh, std::shared_ptr<Renderer> renderer) :
		Transform{ std::move(transform) }, Mesh{ std::move(mesh) }, Renderer{ std::move(renderer) } {}

	void Select() const
	{
		Mesh->Activate();
		Renderer->Activate();
	}

	std::shared_ptr<Transform> Transform;
	std::shared_ptr<Mesh> Mesh;
	std::shared_ptr<Renderer> Renderer;
};
