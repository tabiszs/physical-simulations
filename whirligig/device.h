#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include "Object.h"

class Device
{
public:
	void RenderToTheScreen();
	void RenderToTheTexture(GLint framebufferName);
	void LoadMesh(std::shared_ptr<Object> model);
	void LoadMesh(Object* model);
	void LoadPositionsAndTextureCoords(Object* model);
	void LoadPositionsAndColor(Object* model);
	void LoadUniformBufferObject(int index, int size, void* data);
	void LoadPositionsAndNormals(Object* model);
	unsigned int LoadCubemap(Object* model, std::vector<std::string> faces);
	void UpdateMesh(Object* model);
	void DrawTriangles(Object* model);
	void DrawTriangles(Object* model, int count, int offset);
	void DrawLines(std::shared_ptr<Object> model);
	void DrawLines(Object* model);
	void DrawLines(Object* model, int count, int offset);
	void DrawLinesStrip(Object* model);
	void DrawLinesStrip(Object* model, int count, int offset);
	void DrawLinesAdjacency(Object* model);
	void DrawLinesAdjacency(Object* model, int count, int offset);
	void DrawLineLoop(Object* model);
	void DrawLineLoop(Object* model, int count, int offset);
	void DrawPoints(std::shared_ptr<Object> model);
	void DrawPoints(Object* model);
	void DrawPoints(Object* model, int count, int offset);
	void DrawPatches16(Object* model);
	void DrawPatches16(Object* model, int count, int offset, bool mesh = true);
	void DrawPatches20(Object* model);
	void DrawPatches20(Object* model, int count, int offset);
	void DrawPatches(int points, Object* model, int count, int offset, bool mesh = true);
	void DrawCubemap(Object* model, int cubemapTexture);
	void CleanColor(float color[4]);
	void CreateTexture(GLuint& image_texture);
	void UpdateTexture(GLuint& image_texture, int width, int height, const void* data);
	void BindCubemapTexture(int textureUnit);
private:

};

