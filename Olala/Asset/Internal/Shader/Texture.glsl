#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec4 color;
out vec2 texCoord;
out float texIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

void main()
{
	texCoord = a_TexCoord;
	color = a_Color;
	texIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}

///////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

out vec4 fragColor;

in vec4 color;
in vec2 texCoord;
in float texIndex;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = color;

	switch (int(texIndex))
	{
	case  0: texColor *= texture(u_Textures[ 0], texCoord); break;
	case  1: texColor *= texture(u_Textures[ 1], texCoord); break;
	case  2: texColor *= texture(u_Textures[ 2], texCoord); break;
	case  3: texColor *= texture(u_Textures[ 3], texCoord); break;
	case  4: texColor *= texture(u_Textures[ 4], texCoord); break;
	case  5: texColor *= texture(u_Textures[ 5], texCoord); break;
	case  6: texColor *= texture(u_Textures[ 6], texCoord); break;
	case  7: texColor *= texture(u_Textures[ 7], texCoord); break;
	case  8: texColor *= texture(u_Textures[ 8], texCoord); break;
	case  9: texColor *= texture(u_Textures[ 9], texCoord); break;
	case 10: texColor *= texture(u_Textures[10], texCoord); break;
	case 11: texColor *= texture(u_Textures[11], texCoord); break;
	case 12: texColor *= texture(u_Textures[12], texCoord); break;
	case 13: texColor *= texture(u_Textures[13], texCoord); break;
	case 14: texColor *= texture(u_Textures[14], texCoord); break;
	case 15: texColor *= texture(u_Textures[15], texCoord); break;
	case 16: texColor *= texture(u_Textures[16], texCoord); break;
	case 17: texColor *= texture(u_Textures[17], texCoord); break;
	case 18: texColor *= texture(u_Textures[18], texCoord); break;
	case 19: texColor *= texture(u_Textures[19], texCoord); break;
	case 20: texColor *= texture(u_Textures[20], texCoord); break;
	case 21: texColor *= texture(u_Textures[21], texCoord); break;
	case 22: texColor *= texture(u_Textures[22], texCoord); break;
	case 23: texColor *= texture(u_Textures[23], texCoord); break;
	case 24: texColor *= texture(u_Textures[24], texCoord); break;
	case 25: texColor *= texture(u_Textures[25], texCoord); break;
	case 26: texColor *= texture(u_Textures[26], texCoord); break;
	case 27: texColor *= texture(u_Textures[27], texCoord); break;
	case 28: texColor *= texture(u_Textures[28], texCoord); break;
	case 29: texColor *= texture(u_Textures[29], texCoord); break;
	case 30: texColor *= texture(u_Textures[30], texCoord); break;
	case 31: texColor *= texture(u_Textures[31], texCoord); break;
	}

	fragColor = texColor;
}