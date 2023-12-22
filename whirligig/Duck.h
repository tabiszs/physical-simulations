#pragma once
#include "Object.h"
#include "device.h"

class Duck : public Object
{
public:
	Duck();
	glm::mat4 ModelMatrix() override;
	void LoadMeshTo(std::shared_ptr<Device> device);
	void UpdateMeshTo(std::shared_ptr<Device> device);
	void DrawModelOn(std::shared_ptr<Device> device);
	void Restart();

private:
	void LoadDuck(const std::string& filename);
	void UpdateBuffer();
	void SetInitialPosition();

	std::vector<glm::vec3> vert_vec3;
	std::vector<glm::vec3> norm_vec3;

	std::vector<glm::vec3> vert;
	std::vector<glm::vec3> norm_end_point;

	std::vector<glm::vec3> init_vert;
	std::vector<glm::vec3> init_norm_end_point;
};

