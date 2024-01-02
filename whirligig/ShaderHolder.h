#pragma once
#include <memory>
#include <vector>
#include "shader.h"

class ShaderHolder
{
public:
	static ShaderHolder& Get();
	void Init();
	void Delete();
	std::shared_ptr<Shader> NewSimpleShader();
	std::shared_ptr<Shader> NewCursorShader();

	std::shared_ptr<Shader> cubeShader;
	std::shared_ptr<Shader> planeShader;
	std::shared_ptr<Shader> diagonal_shader;
	std::shared_ptr<Shader> trajectoryShader;

	std::shared_ptr<Shader> boxShader;
	std::shared_ptr<Shader> frameShader;
	std::shared_ptr<Shader> pointShader;
	std::shared_ptr<Shader> bezierShader;
	std::shared_ptr<Shader> computeShader;

	std::shared_ptr<Shader> initial_cursorShader;
	std::shared_ptr<Shader> final_cursorShader;
	std::shared_ptr<Shader> euler_cursorShader;
	std::shared_ptr<Shader> quat_cursorShader;

	std::shared_ptr<Shader> armShader;
	std::shared_ptr<Shader> blockShader;

	std::shared_ptr<Shader> gridShader;

	std::shared_ptr<Shader> oldPointShader;
	std::shared_ptr<Shader> stereoscopyShader;
	std::shared_ptr<Shader> bezierCubeShader;
	std::shared_ptr<Shader> surfaceC0Shader;
	std::shared_ptr<Shader> surfaceC2Shader;
	std::shared_ptr<Shader> gregoryShader;

	std::vector<std::shared_ptr<Shader>> shaders{};

protected:
	explicit ShaderHolder() = default;

	static std::shared_ptr<ShaderHolder> s_instance;
};
