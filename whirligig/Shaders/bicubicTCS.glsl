#version 400 core
layout (vertices = 16) out;

uniform int segmentsU = 16;
uniform int segmentsV = 16;

void main()
{
	gl_TessLevelOuter[0] = gl_TessLevelOuter[2] =  gl_TessLevelInner[1] = segmentsU - 1; 
	gl_TessLevelOuter[1] = gl_TessLevelOuter[3] =  gl_TessLevelInner[0] = segmentsV - 1; 

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}