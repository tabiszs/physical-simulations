#pragma once
#include "Cube.h"
#include <tuple>
#include <list>
#include <set>

class BezierCube : public Cube
{
public:
	BezierCube()
	{
		SetVerticesAndLines();
		SetTreeOfCongruence();
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
	std::array<int, 8> GetCornersPositions();
	void ComputeForce(const std::array<glm::vec3, 8>& cc_corners);

	float dt = 0.001f;
	float gravitation[3] = { 0.0f, 0.0f, 0.0f };
	int use_whole_vector_velocities = 0;
	float mass = 1.0f;
	float k = 4.0f;
	float c1 = 32.0f;
	float c2 = 32.0f;
	bool depth_on;

	std::array<glm::vec3, 64> positions;
	std::array<glm::vec3, 64> velocities;
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	std::shared_ptr<Shader> point_shader;
private:
    void SetVerticesAndLines();
	void SetTreeOfCongruence();
	void UpdateBuffer();
	std::tuple<glm::vec3, glm::vec3> RungeKutta4(const glm::vec3& cc, std::list<std::pair<glm::vec3, bool>>& neighbours, const glm::vec3& x, const glm::vec3& xt);
	std::tuple<glm::vec3, glm::vec3> RungeKutta4(std::list<std::pair<glm::vec3, bool>>& xs, const glm::vec3& x, const glm::vec3& xt);

	const int n = 4;
	const int n_pow_three = n * n * n;
	const float one_div_three = 1.0f / 3.0f;
	const float sqrt_two_div_three = sqrtf(2.0f)/3.0f;
	std::array<std::set<std::pair<int, bool>>, 64> tree_of_congruence_vertices{};
};

