#pragma once
#include "Object.h"
#include <array>
#include "device.h"
#include "Line.h"
#include "Circle.h"

class Hodograph : public Object
{
public:
	Hodograph()
	{
		lineL = std::make_shared<Line>(0.0f, 1.0f, blue);
		lineR = std::make_shared<Line>(0.0f, 1.0f, blue);
		circle = std::make_shared<Circle>(100, glm::vec3( 0,0,0 ), 1.0f, yellow);
		Init();
	}

	void Init();

	void UpdateRadianStep() { rad_step = omega * step; }

	void Update();
	void UpdateL();
	void UpdateR();

	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);

	float& GetAcctualTime() { return time; }
	float GetHistoryTime() { return time - step * plot_size; }
	int& GetOffset() { return idx; }
	int& GetSize() { return plot_size; }
	float* GetTimes() { return times.data(); }
	float* GetPositions() { return positions.data(); }
	float* GetVelocities() { return velocities.data(); }
	float* GetAccelerations() { return accelerations.data(); }
	
	float R = 1.0f; // radius of circle
	float L = 2.0f; // length of arm <R, infinity>
	float step = 0.01f; // simulation step
	float omega = 1.0f;

	std::shared_ptr<Line> lineL;
	std::shared_ptr<Line> lineR;
	std::shared_ptr<Circle> circle;

private:
	float rad_step{};
	float total_rad{};
	glm::vec2 PoP{}; // center point of press
	glm::vec2 dPoP{};
	glm::vec2 ddPoP{};
	glm::vec2 PoC{}; // point on circle
	const glm::vec2 CoC{ 0,0 }; // center of circle

	int idx = 0;
	float time = 0;
	int plot_size = 0;
	const static int SIZE = 1001;
	std::array<float, SIZE> times{};
	std::array<float, SIZE> positions{};
	std::array<float, SIZE> velocities{};
	std::array<float, SIZE> accelerations{};


	void UpdateLines();
	void PositionPoC();
	void PositionPoP();

};

