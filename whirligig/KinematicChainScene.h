#pragma once
#include "scene.h"
#include "Block.h"
#include "Arm.h"

class KinematicChainScene : public Scene
{
public:
	KinematicChainScene(shared_ptr<FpsCamera> c, shared_ptr<Light> l, shared_ptr<ViewFrustrum> vf)
		: Scene(c, l, vf)
	{	
		camera_movement = false;
	}

	void DrawOn(std::shared_ptr<Device> device) override;
	void Update() override;
	void Menu() override;
	void ProcessMouseButtonCallback(int button, int action, int mods, float xpos, float ypos) override;

	std::shared_ptr<Arm> arm1_start;
	std::shared_ptr<Arm> arm2_start;
	std::shared_ptr<Arm> arm1_end;
	std::shared_ptr<Arm> arm2_end;
	std::list<std::shared_ptr<Block>> blocks{};
private:
	void InverseKinematic(float xpos, float ypos, std::list<glm::vec3>& p2s_start, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2);
	glm::vec3 EffectorPositionFrom(float xpos, float ypos, const std::shared_ptr<Arm>& arm2);
	void UseAlternativeSolution(std::list<glm::vec3>& p2s, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2, bool& alt_solution);

	std::list<glm::vec3> p2s_start{};
	std::list<glm::vec3> p2s_end{};
	float length1 = 1.0f;
	float length2 = 1.0f;
	int chain = 0;
	static const int start_chain = 0;
	static const int end_chain = 1;
	bool use_start_alternative_solution = false;
	bool use_end_alternative_solution = false;
};

