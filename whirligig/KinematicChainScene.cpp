#include "KinematicChainScene.h"

void KinematicChainScene::DrawOn(std::shared_ptr<Device> device)
{
	arm1_start->DrawModelOn(device);
	arm1_end->DrawModelOn(device);
	arm2_start->DrawModelOn(device);
	arm2_end->DrawModelOn(device);
}

void KinematicChainScene::Update()
{
	//UpdateJelly();

	std::shared_ptr<Shader> shader;

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
	ImGui::Text("Controls");
	if (ImGui::Button("Start")) {
		start = true;
		//this->cube->SetProperties();
		//this->trajectory->Clean();
		time = glfwGetTime();
		dt = 0;
	}
	if (ImGui::Button("Stop")) {
		start = false;
	}
	if (ImGui::Button("Reset")) {
		start = false;
		//this->cube->SetProperties();
		//this->trajectory->Clean();
	}
	ImGui::Separator();

	ImGui::Text("Robot");
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, start);
	if (ImGui::SliderFloat("Arm1 length", &this->length1, 0.1f, 2.0f))
	{
		arm1_start->SetLength(length1);
		arm1_end->SetLength(length1);
	}
	if(ImGui::SliderFloat("Arm2 length", &this->length2, 0.1f, 2.0f))
	{
		arm2_start->SetLength(length2);
		arm2_end->SetLength(length2);
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
	ImGui::Separator();

	ImGui::Text("Blocks");
	// configuration space texture
	ImGui::PopItemFlag();
	ImGui::End();
}

void KinematicChainScene::ProcessMouseButtonCallback(int button, int action, int mods, float xpos, float ypos)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
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
