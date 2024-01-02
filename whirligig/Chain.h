#pragma once
#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "mat.h"

class Chain: public std::enable_shared_from_this<Chain>
{
public:
	std::shared_ptr<Chain> parent = nullptr;
	std::list<std::shared_ptr<Chain>> children = {};

	void SetParent(std::shared_ptr<Chain> p);
	void AddChild(std::shared_ptr<Chain> c);
	virtual glm::mat4 Frame();
	void UpdateChildren();

private:
	std::shared_ptr<Chain> getptr();
};

