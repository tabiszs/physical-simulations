#version 400 core

const float PI = 3.14159265358979323846;

uniform mat4 invProjViewMtx;
uniform vec2 resolution;
uniform samplerCube cubemap;

uniform float mass;
uniform vec3 blackHolePosition;
uniform float blackHoleDistance;
uniform bool cameraOrbitAroundBlackHole;

out vec4 FragColor;

struct Ray
{
	vec3 origin;
	vec3 dir;
};

Ray GenerateCameraRay()
{
    Ray ray;

    vec2 xy = 2.0 * (gl_FragCoord.xy) / resolution - 1.0;

    vec4 from = invProjViewMtx * vec4(xy, -1, 1);
    vec4 to = invProjViewMtx * vec4(xy, 1, 1);
    from /= from.w;
    to /= to.w;

    ray.origin = from.xyz;
    ray.dir = normalize(to.xyz - from.xyz);

    return ray;
}

Ray GenerateMiddleCameraRay()
{
    Ray ray;

    vec2 xy = vec2(0,0);

    vec4 from = invProjViewMtx * vec4(xy, -1, 1);
    vec4 to = invProjViewMtx * vec4(xy, 1, 1);
    from /= from.w;
    to /= to.w;

    ray.origin = from.xyz;
    ray.dir = normalize(to.xyz - from.xyz);

    return ray;
}

vec3 GetBlackHolePosition()
{
	if(cameraOrbitAroundBlackHole){
        Ray middleRay = GenerateMiddleCameraRay();   
        return middleRay.dir * blackHoleDistance; 
    } else {
        return blackHolePosition;
	}	
}

float PointToRayDistance(vec3 point, Ray ray) 
{
    float t = dot(ray.dir, point - ray.origin) / dot(ray.dir, ray.dir);
    return length(point - (ray.origin + t * ray.dir));
}

float f(float x, float M, float b) 
{
    return 1 - x * x * (1 - 2 * x * M / b);
}

float df(float x, float M, float b) 
{
    return 2 * x * (3 * M * x / b - 1);
}

float g(float x, float M, float b) 
{
    return pow(f(x, M, b), -0.5);
}

// Newton method to find the root of f(x) = 0
float RootSearch(float M, float b) 
{
    float x = 1.0;
    float root;
    int iters = 0;
    
    while(true) {
        iters++;
        root = x - f(x, M, b) / df(x, M, b);
        
        if(abs(root - x) < 0.00001) break;
        if(iters > 1000) return -1.0;
        
        x = root;
    }
    
    return root;
}

// Integrate g(x) from 0 to upper
float Integrate(float upper, float M, float b) 
{
    int N = 1024;
    float dx = upper / N;
    float sum = 0;
    for(int i = 1; i < N; i++) sum += g(i * dx - dx / 2, M, b);
    return sum * dx;
}

mat4 RotationMatrix(vec3 axis, float angle) 
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
    0.0,                                0.0,                                0.0,                                1.0);
}

vec3 RotateVectorAroundAxisByAngle(vec3 vec, vec3 axis, float angle) 
{
    mat4 rot = RotationMatrix(axis, angle);
    return vec3(rot * vec4(vec, 0));
}

void main()
{   
    Ray ray = GenerateCameraRay();
        // FragColor = texture(cubemap, ray.dir); return;                   // DEBUG
    vec3 bhp = GetBlackHolePosition();
    float b = PointToRayDistance(bhp, ray);   
        //FragColor = vec4(b, b/1000, b/1000000, 1.0); return;              // DEBUG
    float upper = RootSearch(mass, b);
        //FragColor = vec4(upper, upper/1000, upper/1000000, 1.0); return;  // DEBUG
    if(upper < 0) {
        FragColor = vec4(0, 0, 0, 1);
        return;
    }
    float angle = 2 * Integrate(upper, mass, b) - PI;
    if(abs(angle) > PI || isnan(angle) || isinf(angle)) {
        FragColor = vec4(0, 0, 0, 1);
    } else {
        vec3 axis = cross(normalize(bhp), normalize(ray.dir));
        vec3 dir = normalize(RotateVectorAroundAxisByAngle(ray.dir, axis, angle));
        FragColor = texture(cubemap, dir);
    }
}