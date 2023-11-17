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
	//void DrawLines(std::shared_ptr<BaseModel> model);
	//void DrawLines(BaseModel* model);
	//void DrawLines(BaseModel* model, int count, int offset);
	//void DrawLinesStrip(BaseModel* model);
	//void DrawLinesStrip(BaseModel* model, int count, int offset);
	//void DrawLinesAdjcency(BaseModel* model);
	//void DrawLinesAdjcency(BaseModel* model, int count, int offset);
	//void DrawPoints(std::shared_ptr<BaseModel> model);
	//void DrawPoints(BaseModel* model);
	//void DrawPoints(BaseModel* model, int count, int offset);
	//void DrawPatches16(BaseModel* model);
	//void DrawPatches16(BaseModel* model, int count, int offset);
	//void DrawPatches20(BaseModel* model);
	//void DrawPatches20(BaseModel* model, int count, int offset);
	//void DrawPatches(int points, BaseModel* model, int count, int offset);
	void CleanColor(float color[4]);
private:

};

