#pragma once
#include <glm/fwd.hpp>
#include "utils.h"

class ViewFrustrum
{
public:
	ViewFrustrum(SIZE viewportSize, float fov, float nearPlane, float farPlane);

	SIZE viewportSize() const { return m_viewportSize; }
	void setViewportSize(SIZE viewportSize);
	float fov() const { return m_fov; }
	void setFov(float fov);
	float nearPlane() const { return m_nearPlane; }
	void setNearPlane(float nearPlane);
	float farPlane() const { return m_farPlane; }
	void setFarPlane(float farPlane);

	glm::vec4 ScreenToClipSpace(int screenPos[2], float zClipPos);
	void Reset();

	glm::mat4  getProjectionMatrix() const;
	constexpr float getWidth() { return m_viewportSize.cx; }
	constexpr float getHeight() { return m_viewportSize.cy; }
	constexpr bool NeedUpdate() { return needUpdate; }
	constexpr void Updated() { needUpdate = false; }

private:
	void CalculatePlanes();
	SIZE m_viewportSize;
	float m_fov;
	float m_nearPlane, m_farPlane;
	float m_leftPlane, m_rightPlane;
	float m_topPlane, m_bottomPlane;
	bool needUpdate = false;
};

