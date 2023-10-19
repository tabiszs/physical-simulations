#version 400 core

out vec4 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelT;
uniform mat4 viewT;
uniform mat4 projectionT;

uniform int width;
uniform int height;
uniform int pixelSize;

uniform float a;
uniform float b;
uniform float c;

uniform float ks;
uniform float m;

uniform vec4 bColor;
uniform vec3 lightPos;  
uniform vec4 lightColor;
uniform vec4 objectColor;

void main() {
   float halfWidth = width / 2.0f / pixelSize;
   float halfHeight = height / 2.0f / pixelSize;
   float x_fragCoord = floor(gl_FragCoord.x / pixelSize);
   float y_fragCoord = floor(gl_FragCoord.y / pixelSize);
   float x = (x_fragCoord- halfWidth) / halfWidth;
   float y = (y_fragCoord - halfHeight) / halfHeight;
   vec4 use_color = bColor;
   mat4 D;
   D[0] = vec4(a, 0, 0, 0);
   D[1] = vec4(0, b, 0, 0);
   D[2] = vec4(0, 0, c, 0);
   D[3] = vec4(0, 0, 0, -1);

   mat4 M = projectionT * viewT * modelT * D * model * view * projection;
   float AA = M[2][2];
   float BB = x * M[0][2] + y * M[1][2] + x * M[2][0] + y * M[2][1] + M[2][3] + M[3][2];
   float CC = x * x * M[0][0] + x * y * M[0][1] + x * M[0][3] + y * x * M[1][0] + y * y * M[1][1] + y * M[1][3] + x * M[3][0] + y * M[3][1] + M[3][3];
   float delta = BB * BB - 4 * AA * CC;

   if(delta >= 0) {
      float sqrt_delta = sqrt(delta);
      float z1 = (-BB + sqrt_delta) / (2 * AA);
      float z2 = (-BB - sqrt_delta) / (2 * AA);
      float z = z1 > z2 ? z1 : z2;

      vec3 norm = normalize(vec3(x,y,z));
      vec3 worldPos = vec3(projection * view * vec4(norm,0.0));
      vec3 lightDir = normalize(lightPos - worldPos);
      vec3 viewDir = normalize(lightPos - worldPos);
      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), m);
      vec4 specular = ks * spec * lightColor;
      vec4 result = specular * objectColor;
      use_color = result;
   }
   FragColor = use_color;
}