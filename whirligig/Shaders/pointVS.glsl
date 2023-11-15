#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 projViewMtx;

void main()
{
	gl_Position = projViewMtx * vec4(aPos, 1.0);
	gl_PointSize = 5.0;
}