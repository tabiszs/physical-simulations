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

void adjustPatch(float u, float v, inout vec3[16] vertices)
{
    vertices[0] = vec3( gl_in[0].gl_Position );
    vertices[1] = vec3( gl_in[1].gl_Position );
    vertices[2] = vec3( gl_in[2].gl_Position );
    vertices[3] = vec3( gl_in[3].gl_Position );
    // if gdy 0
    vertices[4] = vec3( gl_in[4].gl_Position );
    vertices[5] = (u * vec3( gl_in[5].gl_Position ) + v * vec3( gl_in[6].gl_Position ))/(u+v + 0.00001f);
    vertices[6] = ((1-u) * vec3( gl_in[8].gl_Position ) + v * vec3( gl_in[9].gl_Position ))/(1-u+v + 0.00001f);
    vertices[7] = vec3( gl_in[9].gl_Position );
    
    vertices[8] = vec3( gl_in[10].gl_Position );
    vertices[9] = (u*vec3( gl_in[11].gl_Position ) + (1-v) * vec3( gl_in[12].gl_Position ))/(1+u-v + 0.00001f);
    vertices[10] = ((1-u)*vec3( gl_in[14].gl_Position ) + (1-v) * vec3( gl_in[13].gl_Position ))/(2-u-v + 0.00001f);
    vertices[11] = vec3( gl_in[15].gl_Position );
    
    vertices[12] = vec3( gl_in[16].gl_Position );
    vertices[13] = vec3( gl_in[17].gl_Position );
    vertices[14] = vec3( gl_in[18].gl_Position );
    vertices[15] = vec3( gl_in[19].gl_Position );
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

	vec3 vertices[16];
    adjustPatch(u, v, vertices);    
	vec4 pos = vec4( deCasteljauPatch( u, v, vertices), 1. );
	gl_Position = projViewMtx * pos;
}