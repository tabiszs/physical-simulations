#version 400 core
layout (lines_adjacency) in;
layout (line_strip) out;
layout (max_vertices = 256) out;
const int GL_MAX_GEOMETRY_OUTPUT_VERTICES = 256;

uniform mat4 projViewMtx;
uniform vec2 viewport;

vec2 GetScreenCoordinates(vec4 pos)
{
    vec2 coords;
    vec4 clippedCoords =  pos;
    clippedCoords /= clippedCoords.w;
    coords.x = (clippedCoords.x + 1) / 2.0 * viewport.x;
    coords.y = viewport.y - (clippedCoords.y + 1) / 2.0 * viewport.y;
    return coords;
}

int GetDistance(vec2 coords1, vec2 coords2)
{
	float xDist = abs(coords2.x - coords1.x);
	float yDist = abs(coords2.y - coords2.y);
	return int(ceil(sqrt(xDist * xDist + yDist * yDist)));
}

vec4 DeCasteljauAlgorithm4(float t)
{
	vec4 b01 = gl_in[0].gl_Position * (1 - t) + gl_in[1].gl_Position * t;
	vec4 b11 = gl_in[1].gl_Position * (1 - t) + gl_in[2].gl_Position * t;
	vec4 b21 = gl_in[2].gl_Position * (1 - t) + gl_in[3].gl_Position * t;
	vec4 b02 = b01 * (1 - t) + b11 * t;
	vec4 b12 = b11 * (1 - t) + b21 * t;
	return b02 * (1 - t) + b12 * t;
}

void DrawQubicCurve()
{
	int noPoints = 0;
	vec2 coords0 = GetScreenCoordinates(gl_in[0].gl_Position);
	vec2 coords1 = GetScreenCoordinates(gl_in[1].gl_Position);
	vec2 coords2 = GetScreenCoordinates(gl_in[2].gl_Position);
	vec2 coords3 = GetScreenCoordinates(gl_in[3].gl_Position);
	noPoints += GetDistance(coords0, coords1);
	noPoints += GetDistance(coords1, coords2);
	noPoints += GetDistance(coords2, coords3);
	if(noPoints > GL_MAX_GEOMETRY_OUTPUT_VERTICES) {
		noPoints = GL_MAX_GEOMETRY_OUTPUT_VERTICES;
	}		
	
	float t = 0;
	float step = 1.0 / (noPoints-1);
	for (int i = 0; i < noPoints; ++i, t += step) {
		vec4 pos = DeCasteljauAlgorithm4(t);
		gl_Position = pos; 
		EmitVertex();
	}
}

vec4 DeCasteljauAlgorithm3(float t)
{
	vec4 b01 = gl_in[0].gl_Position * (1 - t) + gl_in[1].gl_Position * t;
	vec4 b11 = gl_in[1].gl_Position * (1 - t) + gl_in[2].gl_Position * t;
	return b01 * (1 - t) + b11 * t;
}

void DrawQuadraticCurve()
{
	int noPoints = 0;
	vec2 coords0 = GetScreenCoordinates(gl_in[0].gl_Position);
	vec2 coords1 = GetScreenCoordinates(gl_in[1].gl_Position);
	vec2 coords2 = GetScreenCoordinates(gl_in[2].gl_Position);
	noPoints += GetDistance(coords0, coords1);
	noPoints += GetDistance(coords1, coords2);
	if(noPoints > GL_MAX_GEOMETRY_OUTPUT_VERTICES) {
		noPoints = GL_MAX_GEOMETRY_OUTPUT_VERTICES;
	}		
	
	float t = 0;
	float step = 1.0 / (noPoints-1);
	for (int i = 0; i < noPoints; ++i, t += step) {
		vec4 pos = DeCasteljauAlgorithm3(t);
		gl_Position = pos; 
		EmitVertex();
	}
}

void DrawLinearCurve()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
}

void main() {    
	bool same12 = gl_in[1].gl_Position == gl_in[2].gl_Position;
	bool same23 = gl_in[2].gl_Position == gl_in[3].gl_Position;

	if(same12 && same23)
	{
		
		DrawLinearCurve();
	}
	else if (same23 && !same12)
	{
		DrawQuadraticCurve();
	}
	else
	{
		DrawQubicCurve();
	}
    EndPrimitive();
}