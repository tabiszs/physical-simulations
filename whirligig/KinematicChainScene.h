#pragma once
#include "scene.h"
#include "Block.h"
#include "Arm.h"
#include <queue>
#include <list>

class KinematicChainScene : public Scene
{
public:
	KinematicChainScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf, shared_ptr<Device> device)
		: Scene(c, l, vf)
	{	
		camera_movement = false;
		this->device = device;
		
		arm1_start = make_shared<Arm>(glm::quarter_pi<float>(), 1);
		arm1_start->LoadMeshTo(device);
		arm2_start = make_shared<Arm>(glm::quarter_pi<float>(), 1, arm1_start);
		arm2_start->LoadMeshTo(device);
		arm1_start->SetChild(arm2_start);

		arm1_end = make_shared<Arm>(0, 1);
		arm1_end->LoadMeshTo(device);
		arm2_end = make_shared<Arm>(0, 1, arm1_end);
		arm2_end->LoadMeshTo(device);
		arm1_end->SetChild(arm2_end);

		arm1_animation = make_shared<Arm>(0, 1);
		arm1_animation->LoadMeshTo(device);
		arm2_animation = make_shared<Arm>(0, 1, arm1_animation);
		arm2_animation->LoadMeshTo(device);
		arm1_animation->SetChild(arm2_animation);

		device->CreateTexture(image_texture);
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;
	void ProcessMouseButtonCallback(int button, int action, int mods, float xpos, float ypos) override;
	void UpdateConfigurationSpace();

	std::shared_ptr<Arm> arm1_start;
	std::shared_ptr<Arm> arm2_start;
	std::shared_ptr<Arm> arm1_end;
	std::shared_ptr<Arm> arm2_end;
	std::shared_ptr<Arm> arm1_animation;
	std::shared_ptr<Arm> arm2_animation;
	std::list<std::shared_ptr<Block>> blocks{};
private:
	void InverseKinematic(float xpos, float ypos, std::list<glm::vec3>& p2s_start, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2);
	glm::vec3 EffectorPositionFrom(float xpos, float ypos, const std::shared_ptr<Arm>& arm2);
	void UseAlternativeSolution(std::list<glm::vec3>& p2s, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2, bool& alt_solution);
	void UpdateAnimation();
	void UpdateAnimationArms();
	void UpdateAnimationInConfigurationSpace();

	std::list<glm::vec3> p2s_start{};
	std::list<glm::vec3> p2s_end{};
	float length1 = 1.0f;
	float length2 = 1.0f;
	int chain = 0;
	static const int start_chain = 0;
	static const int end_chain = 1;
	bool use_start_alternative_solution = false;
	bool use_end_alternative_solution = false;
	
	static const int size = 360;
	static const int size_pow_two = size * size;

	//  0 - default - not visited
	// -1 - block
	// >0 - visited
	std::array<int, size_pow_two> configuration_space{};
	GLubyte texture[3 * size_pow_two]{};
	GLuint image_texture;

	float time_start, animation_time = 3.0f;
	bool update_texture = false;
	shared_ptr<Device> device;
	std::queue<int> bfs_queue{};
	std::vector<int> path{};
	int animation_frame;
	bool draw_animation = false;
};

