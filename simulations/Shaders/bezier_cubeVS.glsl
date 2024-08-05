#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_vector;

out vec3 vs_worldPos;
out vec3 vs_normal;
out vec3 vs_viewVec;

uniform mat4 modelMtx;
uniform mat4 projViewMtx;
uniform mat4 invViewMtx;

void main()
{
	gl_Position = vec4(position, 1.0);
	vs_normal = normalize(modelMtx * vec4(normal_vector, 0.0)).xyz;
	vec3 camPos = (invViewMtx * vec4(0,0,0,1)).xyz;
	vs_worldPos = (modelMtx * vec4(position, 1.0)).xyz;
	vs_viewVec = normalize(camPos - vs_worldPos);
}