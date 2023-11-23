#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Object.h"

class Device
{
public:
	void RenderToTheScreen();
	void RenderToTheTexture(GLint framebufferName);
	void LoadMesh(std::shared_ptr<Object> model);
	void LoadMesh(Object* model);
	void LoadPositionsAndTextureCoords(Object* model);
	void UpdateMesh(Object* model);
	void DrawTraingles(Object* model);
	void DrawTraingles(Object* model, int count, int offset);
	void DrawLines(std::shared_ptr<Object> model);
	void DrawLines(Object* model);
	void DrawLines(Object* model, int count, int offset);
	void DrawLinesStrip(Object* model);
	void DrawLinesStrip(Object* model, int count, int offset);
	void DrawLinesAdjcency(Object* model);
	void DrawLinesAdjcency(Object* model, int count, int offset);
	void DrawPoints(std::shared_ptr<Object> model);
	void DrawPoints(Object* model);
	void DrawPoints(Object* model, int count, int offset);
	void DrawPatches16(Object* model);
	void DrawPatches16(Object* model, int count, int offset, bool mesh = true);
	void DrawPatches20(Object* model);
	void DrawPatches20(Object* model, int count, int offset);
	void DrawPatches(int points, Object* model, int count, int offset, bool mesh = true);
	void CleanColor(float color[4]);
private:

};

