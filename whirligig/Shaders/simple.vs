#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMtx;
uniform mat4 projViewMtx;

void main()
{
	gl_Position = projViewMtx * modelMtx * vec4(aPos, 1.0);
}