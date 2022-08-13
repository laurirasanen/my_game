#version 300 es

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 u_proj;
uniform mat4 u_view;

out vec3 v_normal;
out vec2 v_texcoord;

void main()
{
	v_normal = -normalize(mat3(u_view) * a_normal);
	v_texcoord = a_texcoord;

	gl_Position = u_proj * u_view * vec4(a_pos, 1.0);
}