#pragma once
#include "Cube.h"
#include <tuple>
#include <list>
#include <set>
#include "BezierCube.h"

class Jelly : public Cube
{
public:
	Jelly()
	{
		SetVerticesAndLines();
		SetTreeOfCongruence();
		SetCornersPositions();
		SetNeighbours();
		shader = ShaderHolder::Get().pointShader;
		bezier_cube = std::make_shared<BezierCube>(vertices);
	}

	glm::mat4 ModelMatrix() override;
	void LoadMeshTo(std::shared_ptr<Device> device) override;
	void UpdateMeshTo(std::shared_ptr<Device> device) override;
	void DrawModelOn(std::shared_ptr<Device> device) override;
	void DrawPointsOn(std::shared_ptr<Device> device);
	void DrawEdgesOn(std::shared_ptr<Device> device);
	void Update();
	void ComputeForce(const std::array<glm::vec3, 8>& cc_corners);
	void TakeCollisionsIntoAccount(glm::vec3 boundings_dimensions);

	float dt = 0.01f;
	float gravitation[3] = { 0.0f, 0.0f, 0.0f };
	int use_whole_vector_velocities = 0;
	float mass = 1.0f;
	float k = 1.0f;
	float c1 = 1.0f;
	float c2 = 10.0f;
	bool depth_on;

	std::array<glm::vec3, 64> positions{};
	std::array<glm::vec3, 64> velocities{};
	std::array<glm::vec3, 64> dx{};
	std::array<glm::vec3, 64> dxt{};
	std::shared_ptr<BezierCube> bezier_cube;
private:
    void SetVerticesAndLines();
	void SetTreeOfCongruence();
	void SetCornersPositions();
	void SetNeighbours();
	std::array<int, 8> GetCornersPositions();
	void UpdateBuffer();
	void RungeKutta4(int current_idx, const glm::vec3& x, const glm::vec3& xt, const glm::vec3& cc, bool corner);
	void CheckCollisions();

	static const int n = 4;
	static const int n_pow_three = n * n * n;
	const float one_div_three = 1.0f / 3.0f;
	const float sqrt_two_div_three = sqrtf(2.0f)/3.0f;
	
	bool corners_positions[n_pow_three]{};
	int neighbours_count[n_pow_three]{};
	int neighbours_list[18*n_pow_three]{};
	bool neighbours_distance[18 * n_pow_three]{};
	std::array<std::set<std::pair<int, bool>>, 64> tree_of_congruence_vertices{};
};

