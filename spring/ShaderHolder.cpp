#include "ShaderHolder.h"

std::shared_ptr<ShaderHolder> ShaderHolder::s_instance = nullptr;

ShaderHolder& ShaderHolder::Get()
{
	if (!s_instance)
	{
		s_instance = std::shared_ptr<ShaderHolder>(new ShaderHolder());
	}

	return *s_instance;
}

void ShaderHolder::Init()
{
	simpleShader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	pointShader = std::shared_ptr<Shader>(new Shader("shaders/pointVS.glsl", "shaders/pointFS.glsl"));
	oldPointShader = std::shared_ptr<Shader>(new Shader("shaders/point.vs", "shaders/point.fs"));
	bezierShader = std::shared_ptr<Shader>(new Shader("shaders/bezierVS.glsl", "shaders/bezierFS.glsl", "shaders/bezierGS.glsl"));
	stereoscopyShader = std::shared_ptr<Shader>(new Shader("shaders/stereoscopyVS.glsl", "shaders/stereoscopyFS.glsl"));
	surfaceC0Shader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/bicubicTCS.glsl", "shaders/deCasteljauTES.glsl");
	surfaceC2Shader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/bicubicTCS.glsl", "shaders/deBoorTES.glsl");
	gregoryShader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/gregoryTCS.glsl", "shaders/gregoryTES.glsl");
}

void ShaderHolder::Delete()
{
	if (simpleShader != nullptr)	glDeleteProgram(simpleShader->ID);
	if (pointShader != nullptr)		glDeleteProgram(pointShader->ID);
	if (oldPointShader != nullptr)	glDeleteProgram(oldPointShader->ID);
	if (bezierShader != nullptr)	glDeleteProgram(bezierShader->ID);
	if (surfaceC0Shader != nullptr) glDeleteProgram(surfaceC0Shader->ID);
	if (surfaceC2Shader != nullptr) glDeleteProgram(surfaceC2Shader->ID);
	if (gregoryShader != nullptr) glDeleteProgram(gregoryShader->ID);
}
