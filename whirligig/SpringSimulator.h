#pragma once
#include <tuple>
#include <array>
#include <imgui.h>

class FunctionParameters {
public:
	FunctionParameters() {
		static int param_idx;
		i = param_idx++;
	}

	float A = 0;
	float w = 1;
	float phi = 0;
	float t = 2;

	void aGui() { ImGui::PushID("A"+i); ImGui::SliderFloat("A", &(A), -2, 2, "%f", ImGuiSliderFlags_AlwaysClamp); ImGui::PopID(); }
	void wGui() { ImGui::PushID("w" + i); ImGui::SliderFloat("w", &(w), -2, 2, "%f", ImGuiSliderFlags_AlwaysClamp); ImGui::PopID(); }
	void phiGui() { ImGui::PushID("phi" + i); ImGui::SliderFloat("phi", &(phi), -2, 2, "%f", ImGuiSliderFlags_AlwaysClamp); ImGui::PopID(); }
	void tGui() { ImGui::PushID("t" + i); ImGui::SliderFloat("t", &(t), 0, 2, "%f", ImGuiSliderFlags_AlwaysClamp); ImGui::PopID(); }
private:
	int i;
};

class SpringSimulator
{
private:
	float w() 
	{
		return selectable_fun(time, w_fun_type, w_params);
	}
	float f(float q)
	{
		wt = w();
		return bounce * (wt - q);
	}
	float g()
	{
		return -k * v;
	}
	static float sinusoidal(const float& time, const FunctionParameters& params);

	static float selectable_fun(const float& time, const int& type, const FunctionParameters& params);

	float h(float time)
	{
		return selectable_fun(time, h_fun_type, h_params);
	}

	float Force(float q)
	{
		ft = f(q);
		gt = g();
		ht = h(time);
		return ft + gt + ht;
	}

	void EulerMethod()
	{
		a = Force(q) / mass;	
		v += dt * a;
		q += dt * v;
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
	float dt = 0.01;
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
	
	enum FunctionType {
		Constant,
		Sharp,
		Sign,
		Sin
	};
	int h_fun_type = 0;
	int w_fun_type = 0;
	FunctionParameters h_params{};
	FunctionParameters w_params{};

	void Update()
	{
		if (plotSize < SIZE)  {
			++plotSize;
		}
		EulerMethod();
		positions[idx] = q;
		velocities[idx] = v;
		accelerations[idx] = a;
		times[idx] = time;

		fts[idx] = ft;
		gts[idx] = gt;
		hts[idx] = ht;
		wts[idx] = wt;

		idx = (++idx) % SIZE;
		time += dt;
	}

	void Reset()
	{
		idx = 0;
		time = 0;
		plotSize = 0;
	}

	float& GetAcctualTime() { return time; }
	float GetHistoryTime() { return time - dt * plotSize; }
	int& GetOffset() { return idx; }
	int& GetSize() { return plotSize; }
	float* GetFts() { return fts.data(); }
	float* GetGts() { return gts.data(); }
	float* GetHts() { return hts.data(); }
	float* GetWts() { return wts.data(); }
	float* GetTimes() { return times.data(); }
	float* GetPositions() { return positions.data(); }
	float* GetVelocities() { return velocities.data(); }
	float* GetAccelerations() { return accelerations.data(); }
};

