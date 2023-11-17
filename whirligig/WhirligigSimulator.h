#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

class WhirligigSimulator
{
private:
	// Runge-Kutty 4-degree
	glm::mat4 a_ij = glm::transpose(glm::mat4(
		0,	 0,   0, 0,
		0.5, 0,   0, 0,
		0,   0.5, 0, 0,
		0,   0,   1, 0
	));

	glm::vec4 c_i = { 0, 0.5, 0.5, 1 };
	glm::vec4 b_j = { 1. / 6, 1. / 3, 1. / 3, 1. / 6 };

	float RungeKutty4(float x, float t, float (*func)(float, float))
	{
		// k_0 = f(x_n				, t + c_0*dt)
		// k_1 = f(x_n + dt*a_10*k_0, t + c_1*dt)
		// k_2 = f(x_n + dt*a_21*k_1, t + c_2*dt) 
		// k_3 = f(x_n + dt*a_21*k_2, t + c_3*dt)

		float k_0 = func(x					, t			   );
		float k_1 = func(x + dt * 0.5f * k_0, t + 0.5f * dt);
		float k_2 = func(x + dt * 0.5f * k_0, t + 0.5f * dt);
		float k_3 = func(x + dt * 1.0f * k_0, t + 1.0f * dt);
		return x
			+ b_j[0] * k_0
			+ b_j[1] * k_1
			+ b_j[2] * k_2
			+ b_j[3] * k_3;
	}

	int idx = 0;
	float time = 0;
	int plotSize = 0;
	const static int SIZE = 1001;
	std::array<float, SIZE> times;
	std::array<float, SIZE> positions;
	std::array<float, SIZE> velocities;
	std::array<float, SIZE> accelerations;

	std::array<float, SIZE> fts;
	std::array<float, SIZE> gts;
	std::array<float, SIZE> hts;
	std::array<float, SIZE> wts;

public:
	float mass = 1;
	float dt = 0.1;
	float bounce = 1;
	float spring_length = 2;
	float spring_deviation = 2;
	float initial_deviation = 1;
	float v0 = 0;
	float k = 0.1;

	float q = initial_deviation;
	float v = v0;
	float a = 0;

	float ft;
	float gt;
	float ht;
	float wt;
	
};

