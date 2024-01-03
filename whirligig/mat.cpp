#include "mat.h"
#include "utils.h"

glm::mat4 Mat::identity()
{
	return glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

glm::mat4 Mat::translation(glm::vec3 vec)
{
	return translation({ vec, 1.0 });
}

glm::mat4 Mat::translation(glm::vec4 vec)
{
	return glm::mat4(
		1,		0,		0,		0,
		0,		1,		0,		0,
		0,		0,		1,		0,
		vec[0], vec[1], vec[2], 1
	);
}

glm::mat4 Mat::rotationX(float alpha)
{
	float c = cos(alpha);
	float s = sin(alpha);
	return glm::transpose(
		glm::mat4(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		));
}

glm::mat4 Mat::rotationY(float alpha)
{
	float c = cos(alpha);
	float s = sin(alpha);
	return glm::transpose(
		glm::mat4(
			c, 0, -s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1
		));
}

glm::mat4 Mat::rotationZ(float alpha)
{
	float c = cos(alpha);
	float s = sin(alpha);
	return glm::transpose(
		glm::mat4(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		));
}

glm::mat4 Mat::scale(float sx, float sy, float sz)
{
	return glm::mat4(
		sx,  0,  0, 0,
		 0, sy,  0, 0,
		 0,  0, sz, 0,
		 0,  0,  0, 1
	);
}

glm::mat4 Mat::scale(glm::vec3 scale)
{
	return glm::mat4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	);
}

glm::mat4 Mat::perspective(float fovy, float aspect, float zNear, float zFar)
{
	if (fovy <= 0.0f || fovy >= M_PI)
		throw new std::out_of_range("fieldOfView");

	if (zNear <= 0.0f)
		throw new std::out_of_range("nearPlaneDistance");

	if (zFar <= 0.0f)
		throw new std::out_of_range("farPlaneDistance");

	if (zNear >= zFar)
		throw new std::out_of_range("nearPlaneDistance");

	float ctg = 1.0f / tan(fovy * 0.5f);
	float a = ctg / aspect;
	float b = ctg;
	float c = (zFar + zNear) / (zFar - zNear);
	float d = (-2.0f * zFar * zNear) / (zFar - zNear);
	return glm::mat4(
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, -c, -1,
		0, 0, d, 0
	);
}

glm::mat4 Mat::shiftedPerspective(float n, float f, float r, float l, float t, float b)
{
	float r_l = r - l;
	float t_b = t - b;
	float f_n = f - n;
	float x = 2 * n / r_l;
	float y = 2 * n / t_b;
	float z = (f + n) / f_n;
	float w = (-2.0f * f * n) / f_n;
	float i = (r + l) / r_l;
	float j = (t + b) / t_b;
	
	return glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		-i, -j, -z, -1,
		0, 0, w, 0
	);
}

glm::mat4 Mat::powerToBernsteinBaseChangeMtx()
{
	//						| 1,	   0,    0,	 0, |	 1
 	//						| 1,	1/3.,    0,  0, |	 x
	// [b0, b1, b2, b3] =	| 1,	2/3., 1/3.,  0, | *  x^2
	//						| 1,	   1,    1,  1  |    x^3
	//

	return glm::mat4(
		1,	  0,    0,	0,
		1, 1/3.,    0,	0,
		1, 2/3., 1/3.,	0,
		1,    1,    1,	1
	);
}

float Mat::lerp(float a, float b, float t)
{
	t = glm::clamp(t, 0.0f, 1.0f);
	return a * (1 - t) + b * t;
}
