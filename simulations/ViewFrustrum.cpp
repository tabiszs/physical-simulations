#include "ViewFrustrum.h"
#include "mat.h"

ViewFrustrum::ViewFrustrum(SIZE viewportSize, float fov, float nearPlane, float farPlane)
	: m_viewportSize(viewportSize), m_fov(fov), m_nearPlane(nearPlane), m_farPlane(farPlane)
{
	setFov(fov);
	needUpdate = true;
}

void ViewFrustrum::setViewportSize(SIZE viewportSize)
{
	m_viewportSize = viewportSize;
	CalculatePlanes();
	needUpdate = true;
}

void ViewFrustrum::setFov(float fov)
{
	m_fov = fov;
	CalculatePlanes();
	needUpdate = true;
}

void ViewFrustrum::setNearPlane(float nearPlane)
{
	m_nearPlane = nearPlane;
	needUpdate = true;
}

void ViewFrustrum::setFarPlane(float farPlane)
{
	m_farPlane = farPlane;
	needUpdate = true;
}

glm::vec4 ViewFrustrum::ScreenToClipSpace(int screenPos[2], float zClipPos)
{
	float xClipPos = (screenPos[0] / static_cast<float>(m_viewportSize.cx)) * 2 - 1;
	float yClipPos = (screenPos[1] / static_cast<float>(m_viewportSize.cy)) * 2 - 1;
	yClipPos *= -1;
	
	return glm::vec4(xClipPos, yClipPos, zClipPos, 1.0f);
}

void ViewFrustrum::Reset()
{
	setFov(m_fov);
}

glm::mat4 ViewFrustrum::getProjectionMatrix() const
{
	return Mat::shiftedPerspective(m_nearPlane, m_farPlane, m_rightPlane, m_leftPlane, m_topPlane, m_bottomPlane);
}

void ViewFrustrum::CalculatePlanes()
{
	m_topPlane = tan(m_fov / 2) * m_nearPlane;
	m_bottomPlane = -m_topPlane;
	float aspectRatio = m_viewportSize.cx / static_cast<float>(m_viewportSize.cy);
	m_leftPlane = m_bottomPlane * aspectRatio;
	m_rightPlane = m_topPlane * aspectRatio;
}
