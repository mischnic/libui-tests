#version 330 core

uniform vec3 Ambient;
uniform vec3 LightDirection;
uniform ivec3 LightTypes;

in vec3 vColor;
in vec3 vNormal;

out vec4 fColor;

void main() {
	float diffuse = max(0.0, -dot(LightDirection, vNormal));
	float specular = max(0.0, -dot(normalize(LightDirection + vec3(0, 0, -1.0)), vNormal));
	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, 100);

	vec3 lightColor = vec3(1.0);

	vec3 scatteredLight = LightTypes.y * diffuse  * lightColor + LightTypes.x * Ambient;
	vec3 reflectedLight = LightTypes.z * specular * lightColor * 5;

	vec3 rgb = min(vColor.rgb * scatteredLight + reflectedLight, vec3(1.0));
	fColor = vec4(rgb, 1);
	// fColor = vec4(specular, 0, 0, 1.0);
}
