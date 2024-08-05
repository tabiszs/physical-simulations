#version 400 core
layout (vertices = 16) out;

uniform int segmentsU = 16;
uniform int segmentsV = 16;

in vec3 vs_worldPos [];
in vec3 vs_normal [];
in vec3 vs_viewVec [];

out vec3 tcs_worldPos [];
out vec3 tcs_normal [];
out vec3 tcs_viewVec [];

void main()
{
	gl_TessLevelOuter[0] = gl_TessLevelOuter[2] =  gl_TessLevelInner[1] = segmentsU - 1; 
	gl_TessLevelOuter[1] = gl_TessLevelOuter[3] =  gl_TessLevelInner[0] = segmentsV - 1; 

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	tcs_worldPos[gl_InvocationID] = vs_worldPos[gl_InvocationID];
	tcs_normal[gl_InvocationID] = vs_normal[gl_InvocationID];
	tcs_viewVec[gl_InvocationID] = vs_viewVec[gl_InvocationID];

}