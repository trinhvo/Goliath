#version 430

out vec4 FragColor;
in float gDisplacement;
in vec3 gNormal;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{

	vec3 lightDir = normalize(vec3(0,1.0,0));
	vec3 wNormal = normalize(vec3(vec4(gFacetNormal, 1.0) * model));

	float light = max(0.0, dot(lightDir, wNormal));

    FragColor = vec4(0, light / 10.0, light / 5.0, 1.0);
}
