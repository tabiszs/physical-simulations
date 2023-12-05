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
	//VAO Information
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
	//VAO Information
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

void Device::LoadPositionsAndColor(Object* model)
{
	//VAO Information
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Device::LoadUniformBufferObject(int index, int size, void* data)
{
	unsigned int uniform_block;
	glGenBuffers(1, &uniform_block);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_block);
	glBufferData(GL_UNIFORM_BUFFER, 1024, (GLuint*) data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniform_block, 0, 1024);
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
void Device::DrawTriangles(Object* model)
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

void Device::DrawTriangles(Object* model, int count, int offset)
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

void Device::DrawLinesStrip(Object* model)
{
	DrawLinesStrip(model, model->indices.size(), 0);
}

void Device::DrawLinesStrip(Object* model, int count, int offset)
{
	glBindVertexArray(model->VAO);
	glDrawElements(GL_LINE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);
}

void Device::DrawLinesAdjacency(Object* model)
{
	DrawLinesAdjacency(model, model->indices.size(), 0);
}

void Device::DrawLinesAdjacency(Object* model, int count, int offset)
{
	glBindVertexArray(model->VAO);
	glDrawElements(GL_LINES_ADJACENCY, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);
}

void Device::DrawPoints(std::shared_ptr<Object> model)
{
	DrawPoints(model.get());
}

void Device::DrawPoints(Object* model)
{
	DrawPoints(model, model->indices.size(), 0);
}

void Device::DrawPoints(Object* model, int count, int offset)
{
	glBindVertexArray(model->VAO);
	glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);
}

void Device::DrawPatches16(Object* model)
{
	DrawPatches16(model, model->indices.size(), 0);
}

void Device::DrawPatches16(Object* model, int count, int offset, bool mesh)
{
	DrawPatches(16, model, count, offset, mesh);
}

void Device::DrawPatches20(Object* model)
{
	DrawPatches20(model, model->indices.size(), 0);
}

void Device::DrawPatches20(Object* model, int count, int offset)
{
	DrawPatches(20, model, count, offset);
}

void Device::DrawPatches(int points, Object* model, int count, int offset, bool mesh)
{
	if (mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glBindVertexArray(model->VAO);
	glPatchParameteri(GL_PATCH_VERTICES, points);
	glDrawElements(GL_PATCHES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
	glBindVertexArray(0);

	if (mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Device::CleanColor(float color[4])
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
