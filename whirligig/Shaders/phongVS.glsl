#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_vector;

out vec3 worldPos;
out vec3 normal;
out vec3 viewVec;

uniform mat4 modelMtx;
uniform mat4 projViewMtx;
uniform mat4 invViewMtx;

void main()
{
	// gl_Position = projViewMtx * modelMtx * vec4(position, 1.0);
	gl_Position = vec4(position, 1.0);
	normal = normalize(modelMtx * vec4(normal_vector, 0.0)).xyz;
	vec3 camPos = (invViewMtx * vec4(0,0,0,1)).xyz;
	vec3 worldPos = (modelMtx * vec4(position, 1.0)).xyz;
	viewVec = camPos - worldPos;
}