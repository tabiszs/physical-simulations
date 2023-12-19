#version 400 core
layout(quads, equal_spacing, ccw) in;

uniform int segmentsU = 16;
uniform int segmentsV = 16;

uniform mat4 projViewMtx;
uniform mat4 modelMtx;
uniform mat4 invViewMtx;

in vec3 tcs_worldPos [];
in vec3 tcs_normal [];
in vec3 tcs_viewVec [];

out vec3 tes_worldPos;
out vec3 tes_normal;
out vec3 tes_viewVec;

vec3 deCasteljau(vec3 b[4], float t) {
    float t1 = 1.0f - t;

    for (int j = 3; j > 0; --j) {
        for (int i = 0; i < j; ++i) {
            b[i] = b[i] * t1 + b[i + 1] * t;
        }
    }

    return b[0];
}

vec3 deCasteljauPatch(float u, float v, vec3[16] p) {
    vec3 b[4];
    
    for (int i = 0; i < 4; ++i) {
        vec3 tab[4] = { p[4 * i], p[4 * i + 1], p[4 * i + 2], p[4 * i + 3] };
        b[i] = deCasteljau(tab, v);
    }

    return deCasteljau(b, u);
}

float factorial(int k) {
    if (k <= 0) {
        return 1;
    }
    
    float result = 1;
    while (k > 0) {
        result *= k;
        k--;
    }
    return result;
}

float B(int n, int i, float t) {
    float nfactorial = factorial(n);
    float ifactorial = factorial(i);
    float nifactorial = factorial(n-i);
    return (nfactorial/(ifactorial*nifactorial)) * pow(t, i) * pow(1-t,n-1);
}

vec3 BezierNormalVector(float u, float v, vec3[16] p) {

    vec3 dPdu = vec3(0,0,0);
    for (int i=0; i < 3; ++i) {
        for (int j=0; j < 4; ++j) {
            dPdu += 3 * (p[4*(i+1) + j] - p[4*i + j]) * B(3,i,u) * B(4,j,v);
        }
    }

    vec3 dPdv = vec3(0,0,0);
    for (int i=0; i < 4; ++i) {
        for (int j=0; j < 3; ++j) {
            dPdv += 3 * (p[4*i + j+1] - p[4*i + j]) * B(4,i,u) * B(3,j,v);
        }
    }

    dPdu = normalize(dPdu);
    dPdv = normalize(dPdv);
    return abs(normalize(cross(dPdv, dPdu)));
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

	vec3 camPos = (invViewMtx * vec4(0,0,0,1)).xyz;
	tes_worldPos = pos.xyz;
	tes_viewVec = normalize(camPos - tes_worldPos);
    //tes_normal = tcs_normal[0];
    tes_normal = BezierNormalVector(u,v,p);
}