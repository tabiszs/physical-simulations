#version 400 core
layout(location = 0) out vec3 color;

uniform vec4 objectColor;
uniform vec3 lightPosition;

in vec3 tes_worldPos;
in vec3 tes_normal;
in vec3 tes_viewVec;

const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);
const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
const float kd = 0.5, ks = 0.2f, m = 100.0f;

void main() {
	vec3 worldPos = tes_worldPos;
	vec3 normal = normalize(tes_normal);
	vec3 viewVec = normalize(tes_viewVec);
	color = objectColor.rgb * ambientColor;
	
	vec3 lightVec = normalize(lightPosition - worldPos);
	vec3 halfVec = normalize(viewVec + lightVec);
	color += lightColor * objectColor.rgb * kd * clamp(dot(normal, lightVec), 0.0, 1.0); //diffuse color
	float nh = dot(normal, halfVec);
	nh = clamp(nh, 0.0, 1.0);
	nh = pow(nh, m);
	nh *= ks;
	color += lightColor * nh;
	
	color = normal;// clamp(color, 0.0, 1.0);
	//color = objectColor.xyz;
}