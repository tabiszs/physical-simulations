#include "Block.h"

void Block::LoadMeshTo(std::shared_ptr<Device> device)
{
	device->LoadMesh((Object*)this);
	shader->use();
	shader->set4Float("objectColor", color);
	shader->setMatrix4F("modelMtx", ModelMatrix());
}

void Block::UpdateMeshTo(std::shared_ptr<Device> device)
{
	device->UpdateMesh((Object*)this);
}

void Block::DrawModelOn(std::shared_ptr<Device> device)
{
	shader->use();
	device->DrawTriangles((Object*)this);
}

glm::vec3 Block::GetTopLeft()
{
	return {
		vertices[3 * 3 + 0],
		vertices[3 * 3 + 1],
		vertices[3 * 3 + 2]
	};
}

glm::vec3 Block::GetTopRight()
{
	return {
		vertices[3 * 0 + 0],
		vertices[3 * 0 + 1],
		vertices[3 * 0 + 2]
	};
}

glm::vec3 Block::GetBottomLeft()
{
	return {
		vertices[3 * 2 + 0],
		vertices[3 * 2 + 1],
		vertices[3 * 2 + 2]
	};
}

glm::vec3 Block::GetBottomRight()
{
	return {
		vertices[3 * 1 + 0],
		vertices[3 * 1 + 1],
		vertices[3 * 1 + 2]
	};
}

void Block::UpdateBlockSize(const glm::vec3& bottom_right)
{
	const auto& top_left = GetTopLeft();
	if (bottom_right.x >= top_left.x && bottom_right.y <= top_left.y)
	{
		// set right botom
		vertices[3 * 1 + 0] = bottom_right.x;
		vertices[3 * 1 + 1] = bottom_right.y;

		// set left bottom
		vertices[3 * 2 + 0] = top_left.x;
		vertices[3 * 2 + 1] = bottom_right.y;

		// set right top
		vertices[3 * 0 + 0] = bottom_right.x;
		vertices[3 * 0 + 1] = top_left.y;
	}
	this->bottom_right = bottom_right;
	this->bottom_left = GetBottomLeft();
	this->top_right = GetTopRight();
	need_update = true;
}

bool Block::PointIsInside(glm::vec3 pt)
{
	return (top_left.x <= pt.x && pt.x <= top_right.x)
		&& (bottom_left.y <= pt.y && pt.y <= top_left.y);
}

bool Block::HasIntersection(glm::vec3 pt1, glm::vec3 pt2)
{
	if (PointIsInside(pt1) || PointIsInside(pt2))
	{
		return true;
	}

	// pozostaly tylko przypadki, ze oba pkt sa poza prostokatem
	if (pt1.x != pt2.x)
	{
		float a = (pt1.y - pt2.y) / (pt1.x - pt2.x);
		float b = pt1.y - a * pt1.x;

		// przeciecie z lewa krawedzia
		float x = top_left.x;
		float y = a * x + b;
		if (bottom_left.y <= y && y <= top_left.y 
			&& (pt1.x < x && x < pt2.x || pt2.x < x && x < pt1.x))
		{
			return true;
		}

		// przeciecie z prawa krawedzia
		x = top_right.x;
		y = a * x + b;
		if (bottom_left.y <= y && y <= top_left.y 
			&& (pt1.x < x && x < pt2.x || pt2.x < x && x < pt1.x))
		{
			return true;
		}
		
		if (a != 0)
		{
			// przeciecie z dolna krawedzia
			y = bottom_left.y;
			x = (y - b) / a;
			if (top_left.x <= x && x <= top_right.x
				&&(pt1.y < y && y < pt2.y || pt2.y < y && y < pt1.y))
			{
				return true;
			}

			// przeciecie z gorna krawedzia
			y = top_left.y;
			x = (y - b) / a;
			if (top_left.x <= x && x <= top_right.x
				&& (pt1.y < y && y < pt2.y || pt2.y < y && y < pt1.y))
			{
				return true;
			}
		}
		else // pt1.y == pt2.y, czyli rownolegle do x
		{
			// tylko przypadek gdy pt1 i pt2 sa po przeciwnych stronach prostokata
			if (bottom_left.y <= pt1.y && pt1.y <= top_left.y &&
				(pt1.x < top_left.x && top_right.x < pt2.x || pt2.x < top_left.x && top_right.x < pt1.x))
			{
				return true;
			}
		}
	}
	else
	{
		// rownolegle do osi y
		// tylko przypadek gdy pt1 i pt2 sa po przeciwnych stronach prostokata
		if (top_left.x <= pt1.x && pt1.x <= top_right.x &&
			(pt1.y < bottom_left.y && top_left.y < pt2.y || pt2.y < bottom_left.y && top_left.y < pt1.y))
		{
			return true;
		}
	}
	return false;
}
