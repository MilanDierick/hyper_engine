// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 color;
	vec2 tex_coord;
	float tiling_factor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_TexIndex;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.color = a_Color;
	Output.tex_coord = a_TexCoord;
	Output.tiling_factor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 color;
	vec2 tex_coord;
	float tiling_factor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat float v_TexIndex;
layout (location = 4) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = Input.color;

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture(u_Textures[ 0], Input.tex_coord * Input.tiling_factor); break;
		case  1: texColor *= texture(u_Textures[ 1], Input.tex_coord * Input.tiling_factor); break;
		case  2: texColor *= texture(u_Textures[ 2], Input.tex_coord * Input.tiling_factor); break;
		case  3: texColor *= texture(u_Textures[ 3], Input.tex_coord * Input.tiling_factor); break;
		case  4: texColor *= texture(u_Textures[ 4], Input.tex_coord * Input.tiling_factor); break;
		case  5: texColor *= texture(u_Textures[ 5], Input.tex_coord * Input.tiling_factor); break;
		case  6: texColor *= texture(u_Textures[ 6], Input.tex_coord * Input.tiling_factor); break;
		case  7: texColor *= texture(u_Textures[ 7], Input.tex_coord * Input.tiling_factor); break;
		case  8: texColor *= texture(u_Textures[ 8], Input.tex_coord * Input.tiling_factor); break;
		case  9: texColor *= texture(u_Textures[ 9], Input.tex_coord * Input.tiling_factor); break;
		case 10: texColor *= texture(u_Textures[10], Input.tex_coord * Input.tiling_factor); break;
		case 11: texColor *= texture(u_Textures[11], Input.tex_coord * Input.tiling_factor); break;
		case 12: texColor *= texture(u_Textures[12], Input.tex_coord * Input.tiling_factor); break;
		case 13: texColor *= texture(u_Textures[13], Input.tex_coord * Input.tiling_factor); break;
		case 14: texColor *= texture(u_Textures[14], Input.tex_coord * Input.tiling_factor); break;
		case 15: texColor *= texture(u_Textures[15], Input.tex_coord * Input.tiling_factor); break;
		case 16: texColor *= texture(u_Textures[16], Input.tex_coord * Input.tiling_factor); break;
		case 17: texColor *= texture(u_Textures[17], Input.tex_coord * Input.tiling_factor); break;
		case 18: texColor *= texture(u_Textures[18], Input.tex_coord * Input.tiling_factor); break;
		case 19: texColor *= texture(u_Textures[19], Input.tex_coord * Input.tiling_factor); break;
		case 20: texColor *= texture(u_Textures[20], Input.tex_coord * Input.tiling_factor); break;
		case 21: texColor *= texture(u_Textures[21], Input.tex_coord * Input.tiling_factor); break;
		case 22: texColor *= texture(u_Textures[22], Input.tex_coord * Input.tiling_factor); break;
		case 23: texColor *= texture(u_Textures[23], Input.tex_coord * Input.tiling_factor); break;
		case 24: texColor *= texture(u_Textures[24], Input.tex_coord * Input.tiling_factor); break;
		case 25: texColor *= texture(u_Textures[25], Input.tex_coord * Input.tiling_factor); break;
		case 26: texColor *= texture(u_Textures[26], Input.tex_coord * Input.tiling_factor); break;
		case 27: texColor *= texture(u_Textures[27], Input.tex_coord * Input.tiling_factor); break;
		case 28: texColor *= texture(u_Textures[28], Input.tex_coord * Input.tiling_factor); break;
		case 29: texColor *= texture(u_Textures[29], Input.tex_coord * Input.tiling_factor); break;
		case 30: texColor *= texture(u_Textures[30], Input.tex_coord * Input.tiling_factor); break;
		case 31: texColor *= texture(u_Textures[31], Input.tex_coord * Input.tiling_factor); break;
	}

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}
