#version 400 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 projViewMtx;

vec3 deCasteljau(vec3 b[4], float t)
{
    float t1 = 1.0f - t;

    for (int j = 3; j > 0; --j)
    {
        for (int i = 0; i < j; ++i)
        {
            b[i] = b[i] * t1 + b[i + 1] * t;
        }
    }

    return b[0];
}

vec3 deCasteljauPatch(float u, float v, vec3[16] p)
{
    vec3 b[4];
    
    for (int i = 0; i < 4; ++i)
    {
        vec3 tab[4] = { p[4 * i], p[4 * i + 1], p[4 * i + 2], p[4 * i + 3] };
        b[i] = deCasteljau(tab, v);
    }

    return deCasteljau(b, u);
}


void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

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
	vec4 pos = vec4( deCasteljauPatch( u, v, p), 1. );
	gl_Position = projViewMtx * pos;
}