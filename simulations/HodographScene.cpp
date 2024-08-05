#include "HodographScene.h"

void HodographScene::DrawOn(std::shared_ptr<Device> device)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	lineX->DrawModelOn(device);
	lineY->DrawModelOn(device);
	hodograph->DrawModelOn(device);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void HodographScene::Update()
{
	if (start)
	{
		UpdateAnimation();
	}

	hodograph->UpdateMeshTo(device);

	if (camera->NeedUpdate() || viewFrustrum->NeedUpdate())
	{
		UpdateProjViewMtx();

		SetProjViewMtx(hodograph->circle->shader);
		SetProjViewMtx(hodograph->lineL->shader);
		SetProjViewMtx(hodograph->lineR->shader);
		SetProjViewMtx(lineX->shader);
		SetProjViewMtx(lineY->shader);

		camera->Updated();
		viewFrustrum->Updated();
	}
}

void HodographScene::Menu()
{
	ImGui::SetNextWindowSize({ menu_width, viewFrustrum->getHeight() }, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0.0f, 0.0f }, ImGuiCond_Always);
	ImGui::Begin("Hodograf", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Controls");
	{
		if (ImGui::Button("Start")) {
			StartAnimation();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			pause = !pause;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			start = pause = false;
			hodograph->Init();
		}
		ImGui::SliderInt("Speed", &speed, 1, 100);
		ImGui::Separator();
	}

	if (ImGui::SliderFloat("R", &(hodograph->R), 0.1f, hodograph->L))
	{
		hodograph->UpdateR();
	}
	if (ImGui::SliderFloat("L", &(hodograph->L), hodograph->R, 10.0f))
	{
		hodograph->UpdateL();
	}
	if (ImGui::SliderFloat("Omega", &(hodograph->omega), 0, 10))
	{
		hodograph->UpdateRadianStep();
	}
	if (ImGui::SliderFloat("Step", &(hodograph->step), 0.01f, 0.1f))
	{
		hodograph->UpdateRadianStep();
	}
	if (ImGui::SliderFloat("Epsilon", &(hodograph->eps), 0.0f, 0.02f))
	{
		hodograph->UpdateDistribution();
	}
	ImGui::End();

	// Plots
	auto& h = hodograph;
	double t = h->GetAcctualTime();
	double history = 10.0;

	ImGui::Begin("Time Plots");
	auto size = ImVec2(470, 220);
	if (ImPlot::BeginPlot("x(t) Plot", size)) {
		static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
		static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;
		ImPlot::SetupAxes("t", "x", xflags, yflags);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
		ImPlot::SetNextFillStyle({ 1,0,0,1 });
		ImPlot::PlotLine("x(t)", h->GetTimes(), h->GetPositions(), h->GetSize(), 0, h->GetOffset());
		ImPlot::EndPlot();
	}
	if (ImPlot::BeginPlot("dx(t) Plot", size)) {
		static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
		static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;
		ImPlot::SetupAxes("t", "x", xflags, yflags);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
		ImPlot::SetNextFillStyle({ 0,1,0,1 });
		ImPlot::PlotLine("xt(t)", h->GetTimes(), h->GetVelocities(), h->GetSize(), 0, h->GetOffset());
		ImPlot::EndPlot();
	}
	if (ImPlot::BeginPlot("ddx(t) Plot", size)) {
		static ImPlotAxisFlags xflags = ImPlotAxisFlags_None;
		static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;
		ImPlot::SetupAxes("t", "x", xflags, yflags);
		ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
		ImPlot::SetNextFillStyle({ 0,0,1,1 });
		ImPlot::PlotLine("xtt(t)", h->GetTimes(), h->GetAccelerations(), h->GetSize(), 0, h->GetOffset());
		ImPlot::EndPlot();
	}
	ImGui::End();

	ImGui::Begin("Phase state plot");
	if (ImPlot::BeginPlot("State Plot")) {
		static ImPlotAxisFlags xflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;
		static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit;
		ImPlot::SetupAxes("x(t)", "xt(t)", xflags, yflags);
		ImPlot::SetNextFillStyle({ 1,0,0,1 });
		ImPlot::PlotLine("state", h->GetPositions(), h->GetVelocities(), h->GetSize(), 0, h->GetOffset());
		ImPlot::EndPlot();
	}
	ImGui::End();

}

void HodographScene::StartAnimation()
{
	start = true;
	pause = false;
	dt = 0;
	hodograph->Init();
	animation_frame = 0;
	time_start = time = glfwGetTime();
}

void HodographScene::UpdateAnimation()
{
	auto current_time = glfwGetTime();
	auto time_from_start = current_time - time_start;
	auto dfl_dt = (current_time - time);
	if (pause)
	{
		time_start += dfl_dt;
	}
	else
	{
		dt += dfl_dt * speed;
	}

	if (start)
	{
		const auto& step = hodograph->step;
		while (dt > step)
		{
			hodograph->Update();
			dt -= step;
		}
	}

	time = current_time;
}
