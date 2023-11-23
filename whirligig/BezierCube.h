#pragma once
#include "Cube.h"
class BezierCube : public Cube
{
public:
	BezierCube()
	{
		SetVerticlesAndLines();
		shader = ShaderHolder::Get().surfaceC0Shader;
		point_shader = ShaderHolder::Get().pointShader;
	}

	glm::mat4 ModelMatrix() override;
	void LoadMeshTo(std::shared_ptr<Device> device) override;
	void UpdateMeshTo(std::shared_ptr<Device> device) override;
	void DrawModelOn(std::shared_ptr<Device> device) override;
	void DrawPointsOn(std::shared_ptr<Device> device);
	void DrawEdgesOn(std::shared_ptr<Device> device);
	void Update(float t) override;

	float dt = 0.1f;
	float gravitation[3] = { 0.0f, 0.0f, 0.0f };
	int use_whole_vector_velocities = 0;
	float mass = 1.0f;
	float k = 4.0f;
	float c1 = 32.0f;
	float c2 = 32.0f;
	bool depth_on;


	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	std::shared_ptr<Shader> point_shader;
private:
    void SetVerticlesAndLines();
};

