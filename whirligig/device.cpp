#include "device.h"
#include "shader.h"

void Device::RenderToTheScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Device::RenderToTheTexture(GLint framebufferName)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
}

void Device::LoadMesh(std::shared_ptr<Object> model)
{
	LoadMesh(model.get());
}

void Device::LoadMesh(Object* model)
{
	//Binding
	glGenVertexArrays(1, &model->VAO);
	glBindVertexArray(model->VAO);

	//VBO Information
	glGenBuffers(1, &model->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model->vertices.size(), model->vertices.data(), GL_STATIC_DRAW);

	//IBO Information
	glGenBuffers(1, &model->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * model->indices.size(), model->indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Device::LoadPositionsAndTextureCoords(Object* model)
{
	//Binding
	glGenVertexArrays(1, &model->VAO);
	glBindVertexArray(model->VAO);

	//VBO Information
	glGenBuffers(1, &model->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(GLfloat) * model->vertices.size(), model->vertices.data(), GL_STATIC_DRAW);

	//IBO Information
	glGenBuffers(1, &model->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * model->indices.size(), model->indices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Device::UpdateMesh(Object* model)
{
	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model->vertices.size(), model->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * model->indices.size(), model->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Device::DrawTraingles(Object* model)
{
	//Binding
	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);

	//Rendering
	glDrawElements(GL_TRIANGLES, model->indices.size(), GL_UNSIGNED_INT, 0);

	//Unbinding	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Device::DrawTraingles(Object* model, int count, int offset)
{
	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Device::DrawLines(std::shared_ptr<Object> model)
{
	DrawLines(model.get());
}

void Device::DrawLines(Object* model)
{
	DrawLines(model, model->indices.size(), 0);
}

void Device::DrawLines(Object* model, int count, int offset)
{
	glBindVertexArray(model->VAO);
	glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);
}

//void Device::DrawLinesStrip(BaseModel* model)
//{
//	DrawLinesStrip(model, model->GetNoIndices(), 0);
//}
//
//void Device::DrawLinesStrip(BaseModel* model, int count, int offset)
//{
//	glBindVertexArray(model->VAO);
//	glDrawElements(GL_LINE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
//	glBindVertexArray(0);
//}
//
//void Device::DrawLinesAdjcency(BaseModel* model)
//{
//	DrawLinesAdjcency(model, model->GetNoIndices(), 0);
//}
//
//void Device::DrawLinesAdjcency(BaseModel* model, int count, int offset)
//{
//	glBindVertexArray(model->VAO);
//	glDrawElements(GL_LINES_ADJACENCY, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
//	glBindVertexArray(0);
//}
//
//void Device::DrawPoints(std::shared_ptr<BaseModel> model)
//{
//	DrawPoints(model.get());
//}
//
//void Device::DrawPoints(BaseModel* model)
//{
//	DrawPoints(model, model->GetNoIndices(), 0);
//}
//
//void Device::DrawPoints(BaseModel* model, int count, int offset)
//{
//	glBindVertexArray(model->VAO);
//	glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
//	glBindVertexArray(0);
//}
//
//void Device::DrawPatches16(BaseModel* model)
//{
//	DrawPatches16(model, model->GetNoIndices(), 0);
//}
//
//void Device::DrawPatches16(BaseModel* model, int count, int offset)
//{
//	DrawPatches(16, model, count, offset);
//}
//
//void Device::DrawPatches20(BaseModel* model)
//{
//	DrawPatches20(model, model->GetNoIndices(), 0);
//}
//
//void Device::DrawPatches20(BaseModel* model, int count, int offset)
//{
//	DrawPatches(20, model, count, offset);
//}
//
//void Device::DrawPatches(int points, BaseModel* model, int count, int offset)
//{
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glBindVertexArray(model->VAO);
//	glPatchParameteri(GL_PATCH_VERTICES, points);
//	glDrawElements(GL_PATCHES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
//	glBindVertexArray(0);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}

void Device::CleanColor(float color[4])
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
