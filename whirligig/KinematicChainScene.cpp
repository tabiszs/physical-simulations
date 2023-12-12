#include "KinematicChainScene.h"
#include <stack>
#include <algorithm>

void KinematicChainScene::DrawOn(std::shared_ptr<Device> device)
{
	arm1_start->DrawModelOn(device);
	arm1_end->DrawModelOn(device);
	arm2_start->DrawModelOn(device);
	arm2_end->DrawModelOn(device);
	if (draw_animation)
	{
		arm1_animation->DrawModelOn(device);
		arm2_animation->DrawModelOn(device);
	}
	for (auto& block : blocks)
	{
		block->DrawModelOn(device);
	}
}

void KinematicChainScene::Update()
{
	UpdateAnimation();

	if (update_texture)
	{
		device->UpdateTexture(image_texture, size, size, texture);
		update_texture = false;
	}

	if (arm1_start->need_update)
	{
		arm1_start->UpdateMeshTo(device);
		arm1_start->need_update = false;
	}

	if (arm1_end->need_update)
	{
		arm1_end->UpdateMeshTo(device);
		arm1_end->need_update = false;
	}

	if (arm2_start->need_update)
	{
		arm2_start->UpdateMeshTo(device);
		arm2_start->need_update = false;
	}

	if (arm2_end->need_update)
	{
		arm2_end->UpdateMeshTo(device);
		arm2_end->need_update = false;
	}

	if (arm1_animation->need_update)
	{
		arm1_animation->UpdateMeshTo(device);
		arm1_animation->need_update = false;
	}

	if (arm2_animation->need_update)
	{
		arm2_animation->UpdateMeshTo(device);
		arm2_animation->need_update = false;
	}

	for (auto& block : blocks)
	{
		if (block->need_update)
		{
			block->UpdateMeshTo(device);
			block->need_update = false;
		}
	}

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		// czy to potrzebne - co jak dodajemy nowy obiekt
		// tak - na poczatku potem juz nie 
		// a co jesli dodamy nowy obiekt
		// przy tworzeniu trzeba zadbac o dodanie macierzy
		// - lub nie - ona jest na stale w shaderze
		SetProjViewMtx(arm1_start->shader);
		SetProjViewMtx(arm1_end->shader);
		SetProjViewMtx(arm2_start->shader);
		SetProjViewMtx(arm2_end->shader);
		SetProjViewMtx(arm1_animation->shader);
		SetProjViewMtx(arm2_animation->shader);
		for (auto it = blocks.begin(); it != blocks.end(); ++it)
		{
			SetProjViewMtx((*it)->shader);
		}

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void KinematicChainScene::Menu()
{
	ImGui::Begin("Kinematic Chain");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Edit"))
		{
			ImGui::Text("Robot");
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
			if (ImGui::SliderFloat("Arm1 length", &this->length1, 0.1f, 2.0f))
			{
				arm1_start->SetLength(length1);
				arm1_end->SetLength(length1);
				arm1_animation->SetLength(length1);
			}
			if (ImGui::SliderFloat("Arm2 length", &this->length2, 0.1f, 2.0f))
			{
				arm2_start->SetLength(length2);
				arm2_end->SetLength(length2);
				arm2_animation->SetLength(length2);
			}

			ImGui::Text("Start configuration");
			if (ImGui::SliderAngle("Arm1 angle##0", &arm1_start->angle, 0, 360.0f, "%.0f deg", ImGuiSliderFlags_NoInput))
				arm1_start->SetAngle(arm1_start->angle);
			if (ImGui::SliderAngle("Arm2 angle##0", &arm2_start->angle, 0, 360.0f, "%.0f deg", ImGuiSliderFlags_NoInput))
				arm2_start->SetAngle(arm2_start->angle);
			ImGui::RadioButton("Set Effector position##0", &chain, 0);

			ImGui::Text("End configuration");
			if (ImGui::SliderAngle("Arm1 angle##1", &arm1_end->angle, 0, 360.0f, "%.0f deg", ImGuiSliderFlags_NoInput))
				arm1_end->SetAngle(arm1_end->angle);
			if (ImGui::SliderAngle("Arm2 angle##1", &arm2_end->angle, 0, 360.0f, "%.0f deg", ImGuiSliderFlags_NoInput))
				arm2_end->SetAngle(arm2_end->angle);
			ImGui::RadioButton("Set Effector position##1", &chain, 1);
			ImGui::PopItemFlag();

			ImGui::Text("Blocks");
			if (ImGui::Button("Add block"))
			{
				add_new_block = true;
			}
			int idx_to_remove = -1;
			for (int i=0; i< blocks.size(); ++i)
			{
				ImGui::BeginTable("sceneObjects", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBody);
				ImGui::TableNextColumn();
				std::string name = "Block " + std::to_string(blocks[i]->id);
				ImGui::Button(name.c_str());
				ImGui::TableNextColumn();
				name += "qq";
				ImGui::PushID(name.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
				if (ImGui::SmallButton("x"))
				{
					idx_to_remove = i;
				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();
				ImGui::EndTable();
			}
			if (idx_to_remove >= 0)
			{
				blocks.erase(blocks.begin() + idx_to_remove, blocks.begin() + idx_to_remove + 1);
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Pathfinding"))
		{
			if (ImGui::Button("Update configuration space"))
			{
				UpdateConfigurationSpace();
			}
			ImGui::Text("Path exists: %d", path_exists);
			ImGui::Separator();

			if (ImGui::Button("Start")) {
				if (draw_animation)
				{
					start = true;
					time = glfwGetTime();
					dt = 0;
				}
				else
				{
					start = draw_animation = true;
					animation_frame = 0;
					time_start = time = glfwGetTime();
					dt = 0;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause")) {
				start = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset")) {
				start = draw_animation = false;
			}
			ImGui::Separator();
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
			ImGui::SliderFloat("Simulation time", &animation_time, 1, 10);
			ImGui::SliderInt("Simulation speed", &speed, 1, 10);
			ImGui::PopItemFlag();
			ImGui::Separator();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::Image(reinterpret_cast<ImTextureID>(image_texture), ImVec2(size, size));
	ImGui::End();
}

void KinematicChainScene::ProcessMouseCursorPosCallback(GLFWwindow* m_Window, float xpos, float ypos)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && add_new_block)
		{
			// update position new block
			const auto& block = blocks.back();
			auto newClipPos = ScreenToClipSpace(xpos, ypos, clip_space_z);
			auto newWorldPos = ClipToWorldSpace(newClipPos);
			block->UpdateBlockSize(newWorldPos);
		}
	}
	lastX = xpos;
	lastY = ypos;
}

void KinematicChainScene::ProcessMouseButtonCallback(int button, int action, int mods, float xpos, float ypos)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		if (add_new_block)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				// add block
				auto newClipPos = ScreenToClipSpace(xpos, ypos, clip_space_z);
				auto newWorldPos = ClipToWorldSpace(newClipPos);
				std::shared_ptr<Block> block = std::make_shared<Block>(newWorldPos);
				block->LoadMeshTo(device);
				SetProjViewMtx(block->shader);
				blocks.push_back(block);
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				const auto& block = blocks.back();
				add_new_block = false;
			}
		}
		else
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				if (chain == start_chain)
				{
					InverseKinematic(xpos, ypos, p2s_start, arm1_start, arm2_start);
					use_start_alternative_solution = false;
				}
				else
				{
					InverseKinematic(xpos, ypos, p2s_end, arm1_end, arm2_end);
					use_end_alternative_solution = false;
				}
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				if (p2s_start.size() > 1 && chain == start_chain)
				{
					UseAlternativeSolution(p2s_start, arm1_start, arm2_start, use_start_alternative_solution);
				}
				else if (p2s_end.size() > 1 && chain == end_chain)
				{
					UseAlternativeSolution(p2s_end, arm1_end, arm2_end, use_end_alternative_solution);
				}
			}
		}
	}
}

void KinematicChainScene::UpdateConfigurationSpace()
{
	// clear
	for (int i = 0; i < size_pow_two; ++i)
	{
		texture[3 * i] = 0;
		texture[3 * i + 1] = 0;
		texture[3 * i + 2] = 0;
	}

	// get initial angle values
	auto a1 = (int)(arm1_start->angle / glm::two_pi<float>() * 360);
	auto a2 = (int)(arm2_start->angle / glm::two_pi<float>() * 360);
	int start_idx = a1 * size + a2;

	// get final angle values
	a1 = (int)(arm1_end->angle / glm::two_pi<float>() * 360);
	a2 = (int)(arm2_end->angle / glm::two_pi<float>() * 360);
	int end_idx = a1 * size + a2;

	// set blocks
	auto angle1_tmp = arm1_animation->angle;
	auto angle2_tmp = arm2_animation->angle;
	configuration_space.fill(0);
	for (int idx = 0; idx < size_pow_two; ++idx)
	{
		float a1_deg = idx / size;
		float a2_deg = idx % size;
		float a1_rad = a1_deg / 180 * glm::pi<float>();
		float a2_rad = a2_deg / 180 * glm::pi<float>();
		arm1_animation->SetAngle(a1_rad);
		arm2_animation->SetAngle(a2_rad);
		auto pt1 = arm1_animation->GetAnchorPoint();
		auto pt2 = arm2_animation->GetAnchorPoint();
		auto pt3 = arm2_animation->GetEffectorPoint();

		bool arm1_intersected = false;
		bool arm2_intersected = false;
		for (const auto& block : blocks)
		{
			arm1_intersected |= block->HasIntersection(pt1, pt2);
			arm2_intersected |= block->HasIntersection(pt2, pt3);
		}
		if (arm1_intersected || arm2_intersected)
		{
			texture[3 * idx] = 0;
			texture[3 * idx + 1] = 255;
			texture[3 * idx + 2] = 0;
			configuration_space[idx] = blocked_value;
		}
	}
	
	// Flood fill algorithm	
	bfs_queue.push(start_idx);
	configuration_space[start_idx] = 1;
	while (!bfs_queue.empty())
	{
		int idx = bfs_queue.front();
		int counter = configuration_space[idx] + 1;
		a1 = idx / size;
		a2 = idx % size;
		bfs_queue.pop();

		// right
		int w1 = (a1 + 1) % size;
		int w2 = a2;
		int w = (w1 * size + w2);
		if (configuration_space[w] == 0)
		{
			configuration_space[w] = counter;
			bfs_queue.push(w);
		}
		
		// bottom
		w1 = a1;
		w2 = (a2 + 1) % size;
		w = (w1 * size + w2);
		if (configuration_space[w] == 0)
		{
			configuration_space[w] = counter;
			bfs_queue.push(w);
		}

		// left
		w1 = (size + a1 - 1) % size;
		w2 = a2;
		w = (w1 * size + w2);
		if (configuration_space[w] == 0)
		{
			configuration_space[w] = counter;
			bfs_queue.push(w);
		}

		// top
		w1 = a1;
		w2 = (size + a2 - 1) % size;
		w = (w1 * size + w2);
		if (configuration_space[w] == 0)
		{
			configuration_space[w] = counter;
			bfs_queue.push(w);
		}
	}
	
	// fill with gradient
	int distance_from_initial_to_final_pos = configuration_space[end_idx];
	for (int i = 0; i < size_pow_two; ++i)
	{
		const auto& val = std::min(configuration_space[i], distance_from_initial_to_final_pos);
		if (val > 0)
		{
			int col = (float)(distance_from_initial_to_final_pos - val) / distance_from_initial_to_final_pos * 255;
			texture[3 * i] = col;
			texture[3 * i + 1] = col;
			texture[3 * i + 2] = col;
		}
	}

	// retrive path - reverse path - from end to start
	path.clear();
	if (configuration_space[end_idx] < 1)
	{
		path_exists = false;
	}
	else
	{
		path.push_back(end_idx);
		while (configuration_space[path.back()] != 1) // top != start_idx
		{
			int idx = path.back();
			const auto& wanted_val = configuration_space[path.back()] - 1;
			a1 = idx / size;
			a2 = idx % size;

			int w1 = ((a1 + 1) % size) * size + a2;
			int w2 = a1 * size + ((a2 + 1) % size);
			int w3 = ((size + a1 - 1) % size) * size + a2;
			int w4 = a1 * size + ((size + a2 - 1) % size);
			if (configuration_space[w1] == wanted_val)
			{
				path.push_back(w1);
			}
			else if (configuration_space[w2] == wanted_val)
			{
				path.push_back(w2);
			}
			else if (configuration_space[w3] == wanted_val)
			{
				path.push_back(w3);
			}
			else if (configuration_space[w4] == wanted_val)
			{
				path.push_back(w4);
			}
			else
			{
				throw new exception("error in retriving path");
			}
		}
		std::reverse(path.begin(), path.end());
		path_exists = true;
	}

	// set path on texture
	for(const auto& idx : path)
	{
		texture[3 * idx] = 0;
		texture[3 * idx + 1] = 0;
		texture[3 * idx + 2] = 255;
	}

	// set initial pos
	texture[3 * start_idx] = 255;
	texture[3 * start_idx + 1] = 0;
	texture[3 * start_idx + 2] = 0;

	// set final pos
	texture[3 * end_idx] = 255;
	texture[3 * end_idx + 1] = 0;
	texture[3 * end_idx + 2] = 0;

	update_texture = true;
}

void KinematicChainScene::InverseKinematic(float x_sceen_pos, float y_screen_pos, 
	std::list<glm::vec3>& p2s, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2)
{

	const auto& new_p3 = EffectorPositionFrom(x_sceen_pos, y_screen_pos, arm2);
	/// Metody rozwiazywania kinematyki odwrotnej
	// 1. sposob
	// ograniczeniami sa dwa rownania okregu
	// odejmujemy je od siebie
	// wyznaczamy jedna niewiadama i stosujemy podstawienie do jednego z rownan okregu
	// rozwiazaujemy rownanie kwadratowe
	//
	// 2. sposob
	// z przeksztalcania ukladow F2 = F0*F02
	//
	// 3. sposob
	// niewiadomymi sa katy
	// x' = x0 + l1 cos(a) + l2 cos(a+b)
	// y' = y0 + l1 sin(a) + l2 sin(a+b)
	// przeksztalcamy, zeby fun(a+b) byla po jednej stronie
	// podnosimy do kwadratu
	// dodajemy
	// stosujemy podstawienie A = x0 + l1 cos(a), B = y0 + l1 sin(a)
	// dzielimy stronami przez sqrt(A^2+B^2)
	//
	// 4. sposob
	// z tw. cosinusow
	// zal. l1, l2 > 0
	// co jesli l=0? -> dostajemy NaN
	// nie dokladne obliczenia
	/*
	p2s.clear();
	const auto& p1 = arm1->anchor_point;
	const auto& p3 = new_p3;
	const auto& l = sqrtf(glm::dot(p3 - p1, p3 - p1));
	const auto& q1 = atan2f(p3.y - p1.y, p3.x - p1.x) - acosf((length1 * length1 + l * l - length2 * length2) / (2 * l * length2));
	const auto& q2 = glm::pi<float>() - acosf((length1 * length1 + length2 * length2 - l * l) / (2 * length1 * length2));
	arm1->SetAngle(q1);
	arm2->SetAnchorPos(arm1->GetEffectorPoint());
	arm2->SetAngle(q2);
	*/

	p2s.clear();
	const auto& p1 = arm1->GetAnchorPoint();
	const auto& p3 = new_p3;
	const auto& x = -p1.x * p1.x - p1.y * p1.y + p3.x * p3.x + p3.y * p3.y + length1 * length1 - length2 * length2;
	const auto& y = -2 * (p3.y - p1.y);
	const auto& z = 2 * (p3.x - p1.x);
	float a = y * y + z * z;
	float b = 2 * x * y - 2 * p1.x * z * y - 2 * z * z * p1.y;
	float c = x * x - 2 * p1.x * z * x + z * z * (p1.x * p1.x + p1.y * p1.y - length1 * length1);
	float delta = b * b - 4 * a * c;
	float sqrt_delta = sqrtf(delta);
	if (z == 0)				// 0 lub nieskocznosc rozwiazan
	{						// czyli p3x == p1.x 	
		return;
	}

	if (delta > 0)			// 2 rozwiazania
	{						
		float p2y1 = (-b + sqrt_delta) / (2 * a);
		float p2y2 = (-b - sqrt_delta) / (2 * a);
		float p2x1 = (x + y * p2y1) / z;
		float p2x2 = (x + y * p2y2) / z;
		p2s.push_back({ p2x1, p2y1, 0.0f });
		p2s.push_back({ p2x2, p2y2, 0.0f });
	}
	else if (delta == 0)	// 1 rozwiazanie
	{
		float p2y = -0.5 * b / a;
		float p2x = (x + y * p2y) / z; // co jesli p3x==p1.x? -> dla delty=0 taka sytuacja nie wystapi
		p2s.push_back({ p2x, p2y, 0.0f });
	}
	else					// 0 rozwiazan
	{
		return;
	}

	arm1->SetEffectorPos(p2s.front());
	arm1->UpdateAngle();
	arm2->SetAnchorPos(p2s.front());
	arm2->SetEffectorPos(new_p3);
	arm2->UpdateAngle();
}

glm::vec3 KinematicChainScene::EffectorPositionFrom(float x_sceen_pos, float y_screen_pos, const std::shared_ptr<Arm>& arm2)
{
	auto worldPos = arm2->GetEffectorPoint();
	auto clippedCoords = WorldToClipSpace({ worldPos, 1.0f });
	auto newClipPos = ScreenToClipSpace(x_sceen_pos, y_screen_pos, clippedCoords.z);
	auto newWorldPos = ClipToWorldSpace(newClipPos);
	return newWorldPos;
}

void KinematicChainScene::UseAlternativeSolution(std::list<glm::vec3>& p2s, const std::shared_ptr<Arm>& arm1, const std::shared_ptr<Arm>& arm2, bool& alt_solution)
{
	if (alt_solution)
	{
		arm1->SetEffectorPos(p2s.front());
		arm2->SetAnchorPos(p2s.front());
	}
	else
	{
		arm1->SetEffectorPos(p2s.back());
		arm2->SetAnchorPos(p2s.back());
	}
	alt_solution = !alt_solution;
}

void KinematicChainScene::UpdateAnimation()
{
	auto current_time = glfwGetTime();
	auto time_from_start = current_time - time_start;
	auto dfl_dt = (current_time - time);
	dt += dfl_dt * speed;
	if (start && (time_from_start - dfl_dt) <= animation_time)
	{
		const auto& step = animation_time / path.size();
		while (dt > step)
		{
			if (animation_frame < path.size())
			{
				UpdateAnimationArms();
				UpdateAnimationInConfigurationSpace();
				animation_frame++;
			}
			dt -= step;
		}
	}
	time = current_time;
}

void KinematicChainScene::UpdateAnimationArms()
{
	int idx = path[animation_frame];
	float angle1_deg = idx / size;
	float angle2_deg = idx % size;
	float angle1_rad = angle1_deg / 180 * glm::pi<float>();
	float angle2_rad = angle2_deg / 180 * glm::pi<float>();

	arm1_animation->SetAngle(angle1_rad);
	arm2_animation->SetAngle(angle2_rad);
}

void KinematicChainScene::UpdateAnimationInConfigurationSpace()
{
	int idx = path[animation_frame];
	texture[3 * idx] = 255;
	texture[3 * idx + 1] = 0;
	texture[3 * idx + 2] = 0;

	if (animation_frame > 1) // initial position is still red
	{
		idx = path[animation_frame - 1];
		texture[3 * idx] = 0;
		texture[3 * idx + 1] = 0;
		texture[3 * idx + 2] = 255;
	}

	update_texture = true;
}
