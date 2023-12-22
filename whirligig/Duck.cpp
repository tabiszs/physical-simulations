#include "Duck.h"

Duck::Duck()
{
	LoadDuck("Models/duck.txt");
	// set duck shader
}

glm::mat4 Duck::ModelMatrix()
{
	return Mat::identity();
}

void Duck::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadPositionsAndNormals((Object*)this);
	shader->use();
	shader->set4Float("objectColor", blue);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Duck::UpdateMeshTo(std::shared_ptr<Device> device)
{
	UpdateBuffer();
	device->UpdateMesh((Object*)this);
}

void Duck::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawPoints((Object*)this);
}

void Duck::Restart()
{
	SetInitialPosition();
	need_update = true;
}

void Duck::LoadDuck(const std::string& filename)
{
	std::ifstream is(filename, std::ifstream::in);
	if (!is.is_open())
		return;

	std::string line;
	std::getline(is, line);
	int vertCount = std::stoi(line);

	std::vector<float> vert;
	std::vector<float> norm;
	std::vector<float> tex;
	for (int i = 0; i < vertCount; i++)
	{
		std::getline(is, line);
		std::stringstream ss(line);
		std::string p1, p2, p3, n1, n2, n3, t1, t2;

		ss >> p1;
		ss >> p2;
		ss >> p3;
		vert.push_back(std::stof(p1));
		vert.push_back(std::stof(p2));
		vert.push_back(std::stof(p3));
		vert_vec3.push_back(glm::vec3(std::stof(p1), std::stof(p2), std::stof(p3)));

		ss >> n1;
		ss >> n2;
		ss >> n3;
		norm.push_back(std::stof(n1));
		norm.push_back(std::stof(n2));
		norm.push_back(std::stof(n3));
		norm_vec3.push_back(glm::vec3(std::stof(n1), std::stof(n2), std::stof(n3)));

		ss >> t1;
		ss >> t2;
		tex.push_back(std::stof(t1));
		tex.push_back(std::stof(t2));
	}
	std::getline(is, line);
	int triangleCount = std::stoi(line);

	std::vector<int> inds;
	for (int i = 0; i < triangleCount; i++)
	{
		getline(is, line);
		std::stringstream ss(line);
		std::string f1, f2, f3;
		ss >> f1;
		ss >> f2;
		ss >> f3;
		inds.push_back(std::stoi(f1));
		inds.push_back(std::stoi(f2));
		inds.push_back(std::stoi(f3));
	}

	is.close();

	// scale to cube 0-1
	glm::vec3 max{-INFINITY, -INFINITY, -INFINITY }, min{INFINITY, INFINITY, INFINITY};
	for (int i = 0; i < vert_vec3.size(); ++i)
	{
		const auto& p = vert_vec3[i];
		if (p.x > max.x)
		{
			max.x = p.x;
		}
		if (p.y > max.y)
		{
			max.y = p.y;
		}
		if (p.z > max.z)
		{
			max.z = p.z;
		}
		if (p.x < min.x)
		{
			min.x = p.x;
		}
		if (p.y < min.y)
		{
			min.y = p.y;
		}
		if (p.z < min.z)
		{
			min.z = p.z;
		}
	}
	const auto d_max_min = max - min;
	float scale_factor = 1.0f / glm::max(d_max_min.x, glm::max(d_max_min.y, d_max_min.z));

	// translate to beginning in 0,0,0
	if (min.x < 0)
	{
		for (int i = 0; i < vert_vec3.size(); ++i)
		{
			vert_vec3[i].x -= min.x;
		}
	}
	if (min.y < 0)
	{
		for (int i = 0; i < vert_vec3.size(); ++i)
		{
			vert_vec3[i].y -= min.y;
		}
	}
	if (min.z < 0)
	{
		for (int i = 0; i < vert_vec3.size(); ++i)
		{
			vert_vec3[i].z -= min.z;
		}
	}

	for (int i = 0; i < vert_vec3.size(); ++i)
	{
		init_vert.push_back(vert_vec3[i]);

		// wektor normalny deformujemy tak jak pkt i potem odzyskujemy w shaderze
		glm::vec3 norm_end_point = vert_vec3[i] + norm_vec3[i];
		init_norm_end_point.push_back(norm_end_point);
	}
	this->vert.reserve(init_vert.size());
	norm_end_point.reserve(init_norm_end_point.size());

	for (int i = 0; i < inds.size(); ++i) 
	{
		indices.push_back(inds[i]);
	}
}

void Duck::UpdateBuffer()
{
	for (int i = 0; i < vert_vec3.size(); ++i)
	{
		vertices[6 * i + 0] = vert_vec3[i].x;
		vertices[6 * i + 1] = vert_vec3[i].y;
		vertices[6 * i + 2] = vert_vec3[i].z;
		vertices[6 * i + 3] = norm_vec3[i].x;
		vertices[6 * i + 4] = norm_vec3[i].y;
		vertices[6 * i + 5] = norm_vec3[i].z;
	}
}

void Duck::SetInitialPosition()
{
	for (int i = 0; i < vert.size(); ++i)
	{
		vertices.push_back(init_vert[i].x);
		vertices.push_back(init_vert[i].y);
		vertices.push_back(init_vert[i].z);

		// wektor normalny deformujemy tak jak pkt i potem odzyskujemy w shaderze
		vertices.push_back(init_norm_end_point[i].x);
		vertices.push_back(init_norm_end_point[i].y);
		vertices.push_back(init_norm_end_point[i].z);

		// pos_x, pos_y, pos_z, norm_end_pos_x, norm_end_pos_y, norm_end_pos_z
	}
}
