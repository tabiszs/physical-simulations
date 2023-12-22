#include "Grid.h"

glm::mat4 Grid::ModelMatrix()
{
	return Mat::translation(position);
}

void Grid::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", grey);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Grid::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawLines((Object*)this);
}

void Grid::GenerateMesh()
{
	const float noIndices = 2 * (xSize + zSize + 2);
	vertices.reserve(3 * noIndices);
	indices.reserve(noIndices);

	for (int i = 0; i <= xSize; ++i)
	{
		// begin line
		vertices.push_back(i);		// x
		vertices.push_back(0);		// y
		vertices.push_back(0);		// z

		// end line
		vertices.push_back(i);		// x
		vertices.push_back(0);		// y
		vertices.push_back(zSize);	// z
	}

	for (int i = 0; i <= zSize; ++i)
	{
		// begin line
		vertices.push_back(0);		// x
		vertices.push_back(0);		// y
		vertices.push_back(i);		// z

		// end line
		vertices.push_back(xSize);	// x
		vertices.push_back(0);		// y
		vertices.push_back(i);		// z
	}

	for (int i = 0; i < noIndices; ++i)
	{
		indices.push_back(i);
	}
}
