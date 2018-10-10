#version 330 core

uniform mat4 ProjectionM;
uniform mat4 ModelViewM;
uniform mat3 NormalM;

in vec3 in_position;
in vec3 in_color;
in vec3 in_normal;

out vec3 vColor;
out vec3 vNormal;

void main() {
	gl_Position = ProjectionM * ModelViewM * vec4(in_position, 1.0);
	vNormal = normalize(NormalM * in_normal);
	vColor = in_color;
}
