#pragma once
#include "Cursor.h"
#include "Chain.h"

class PumaEffector : public Cursor, public Chain
{
public: 
	PumaEffector() : Cursor(ShaderHolder::Get().NewCursorShader())
	{

	}

	glm::mat4 ModelMatrix() const override;
	void LoadMeshTo(std::shared_ptr<Device> device) override;
	void UpdateTo(std::shared_ptr<Device> device);
};

