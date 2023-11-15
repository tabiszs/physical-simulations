#version 400 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 projViewMtx;
float parmateres[4] = {-1, 0, 1, 2};

// k - degree
// n+1 - control points
// m+1 - knots: 0, 1, 2, ...
// m = n + k + 1
// b_j^r - auxiliary knot at index j with degree r
// b_3^3 - new knot (position) on curve for k=3
vec3 deBoor(vec3 b[4], float u)
{
    for(int r = 1; r <= 3; ++r)
    {
        for(int j = 3; j >=r; --j)
        {
            float w = (u-j)/(j+3-(r-1) - j);
            b[j] = mix(b[j-1], b[j], w);
        }
    }
    return b[3];
}

vec3 deBoorPatch(float u, float v, vec3[16] p)
{
    vec3 b[4];
    
    for (int i = 0; i < 4; ++i)
    {
        vec3 tab[4] = { p[4 * i], p[4 * i + 1], p[4 * i + 2], p[4 * i + 3] };
        b[i] = deBoor(tab, v);
    }

    return deBoor(b, u);
}


void main()
{
    float u = gl_TessCoord.x + 3;
    float v = gl_TessCoord.y + 3;

	vec3 p[16] = {
      vec3( gl_in[0].gl_Position ),
      vec3( gl_in[1].gl_Position ),
      vec3( gl_in[2].gl_Position ),
      vec3( gl_in[3].gl_Position ),

      vec3( gl_in[4].gl_Position ),
      vec3( gl_in[5].gl_Position ),
      vec3( gl_in[6].gl_Position ),
      vec3( gl_in[7].gl_Position ),

      vec3( gl_in[8].gl_Position ),
      vec3( gl_in[9].gl_Position ),
      vec3( gl_in[10].gl_Position ),
      vec3( gl_in[11].gl_Position ),

      vec3( gl_in[12].gl_Position ),
      vec3( gl_in[13].gl_Position ),
      vec3( gl_in[14].gl_Position ),
      vec3( gl_in[15].gl_Position ),

    };
	vec4 pos = vec4( deBoorPatch( u, v, p), 1. );
	gl_Position = projViewMtx * pos;
}