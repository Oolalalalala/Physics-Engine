#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_LocalQuadCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;

out vec4 color;
out vec2 localCoord;
out float thickness;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

void main()
{
	localCoord = a_LocalQuadCoord;
	color = a_Color;
	thickness = a_Thickness;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

out vec4 fragColor;

in vec4 color;
in vec2 localCoord;
in float thickness;

void main()
{
	float dist2 = dot(localCoord, localCoord);
	fragColor = color * (1.f - smoothstep(1.f, 1.1f, dist2)) * 
		        smoothstep((0.9487f - thickness) * (0.9487f - thickness), (1.f - thickness) * (1.f - thickness), dist2);
}
