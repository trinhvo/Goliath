#version 330 core
out vec4 color;
in vec2 ourUv;
in vec3 ourNormal;
in vec3 ourPosition;

void main()
{
	vec3 normalDir = normalize(ourNormal);
	vec3 lightDir = normalize(vec3(1,1,0));
	float ndotl = dot(normalDir, lightDir);
	float diffuse = max(0.0, ndotl);
	color = vec4(vec3(1.0, 1.0, 1.0) * ndotl, 1.0);
}




 


