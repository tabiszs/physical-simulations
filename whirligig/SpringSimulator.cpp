#include "SpringSimulator.h"
#include <glm/glm.hpp>

float SpringSimulator::sinusoidal(const float& time, const FunctionParameters& params)
{
	return params.A * sinf(params.w * time + params.phi);
}

float SpringSimulator::selectable_fun(const float& time, const int& type, const FunctionParameters& params)
{
	switch (type)
	{
	case Constant:
		return params.A;
		break;
	case Sharp:
		return time < params.t ? 0 : params.A;
		break;
	case Sign:
		return glm::sign(sinusoidal(time, params));
		break;
	case Sin:
		return sinusoidal(time, params);
		break;
	}
}