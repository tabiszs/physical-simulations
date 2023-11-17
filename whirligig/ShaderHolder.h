#pragma once
#include <memory>
#include "shader.h"

class ShaderHolder
{
public:
	static ShaderHolder& Get();
	void Init();
	void Delete();

	std::shared_ptr<Shader> cubeShader;
	std::shared_ptr<Shader> planeShader;
	std::shared_ptr<Shader> diagonalShader;
	std::shared_ptr<Shader> trajectoryShader;
	std::shared_ptr<Shader> pointShader;
	std::shared_ptr<Shader> oldPointShader;
	std::shared_ptr<Shader> bezierShader;
	std::shared_ptr<Shader> stereoscopyShader;
	std::shared_ptr<Shader> surfaceC0Shader;
	std::shared_ptr<Shader> surfaceC2Shader;
	std::shared_ptr<Shader> gregoryShader;

protected:
	explicit ShaderHolder() = default;

	static std::shared_ptr<ShaderHolder> s_instance;
};
