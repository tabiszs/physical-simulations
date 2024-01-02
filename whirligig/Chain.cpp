#include "Chain.h"
#include "Object.h"

void Chain::SetParent(std::shared_ptr<Chain> p)
{
	p->AddChild(this->getptr());
	parent = p;
}

void Chain::AddChild(std::shared_ptr<Chain> c)
{
	children.push_back(c);
}

glm::mat4 Chain::Frame()
{
	return Mat::identity();
}

void Chain::UpdateChildren()
{
	for (const auto& child : children)
	{
		const auto& c = std::dynamic_pointer_cast<Object>(child);
		c->need_update = true;
		child->UpdateChildren();
	}
}

std::shared_ptr<Chain> Chain::getptr()
{
	return shared_from_this();
}
