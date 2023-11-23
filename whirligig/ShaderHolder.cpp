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
	cubeShader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	planeShader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	diagonal_shader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	trajectoryShader = std::shared_ptr<Shader>(new Shader("shaders/pointVS.glsl", "shaders/pointFS.glsl"));
	
	boxShader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	frameShader = std::shared_ptr<Shader>(new Shader("shaders/simple.vs", "shaders/simple.fs"));
	pointShader = std::shared_ptr<Shader>(new Shader("shaders/pointSizeVS.glsl", "shaders/pointFS.glsl"));
	surfaceC0Shader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/bicubicTCS.glsl", "shaders/deCasteljauTES.glsl");

	cursorShader = std::make_shared<Shader>("shaders/cursorVS.glsl", "shaders/cursorFS.glsl");

	oldPointShader = std::shared_ptr<Shader>(new Shader("shaders/point.vs", "shaders/point.fs"));
	bezierShader = std::shared_ptr<Shader>(new Shader("shaders/bezierVS.glsl", "shaders/bezierFS.glsl", "shaders/bezierGS.glsl"));
	stereoscopyShader = std::shared_ptr<Shader>(new Shader("shaders/stereoscopyVS.glsl", "shaders/stereoscopyFS.glsl"));
	surfaceC2Shader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/bicubicTCS.glsl", "shaders/deBoorTES.glsl");
	gregoryShader = std::make_shared<Shader>("shaders/bicubicVS.glsl", "shaders/bicubicFS.glsl", "shaders/gregoryTCS.glsl", "shaders/gregoryTES.glsl");
}

void ShaderHolder::Delete()
{
	if (cubeShader != nullptr)	glDeleteProgram(cubeShader->ID);
	if (planeShader != nullptr)	glDeleteProgram(planeShader->ID);
	if (diagonal_shader != nullptr)	glDeleteProgram(diagonal_shader->ID);
	if (trajectoryShader != nullptr)	glDeleteProgram(trajectoryShader->ID);

	if (boxShader != nullptr)	glDeleteProgram(boxShader->ID);
	if (frameShader != nullptr)	glDeleteProgram(frameShader->ID);
	if (pointShader != nullptr)		glDeleteProgram(pointShader->ID);
	if (surfaceC0Shader != nullptr) glDeleteProgram(surfaceC0Shader->ID);

	if (cursorShader != nullptr) glDeleteProgram(cursorShader->ID);

	if (oldPointShader != nullptr)	glDeleteProgram(oldPointShader->ID);
	if (bezierShader != nullptr)	glDeleteProgram(bezierShader->ID);
	if (surfaceC2Shader != nullptr) glDeleteProgram(surfaceC2Shader->ID);
	if (gregoryShader != nullptr) glDeleteProgram(gregoryShader->ID);
}
