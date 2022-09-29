//--------------------------
// - Hazel 2D -
// renderer_2d Circle Shader
// --------------------------

#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 local_position;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.local_position = a_LocalPosition;
	Output.color = a_Color;
	Output.thickness = a_Thickness;
	Output.fade = a_Fade;

	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 local_position;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.local_position);
    float circle = smoothstep(0.0, Input.fade, distance);
    circle *= smoothstep(Input.thickness + Input.fade, Input.thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_Color = Input.color;
	o_Color.a *= circle;

	o_EntityID = v_EntityID;
}
