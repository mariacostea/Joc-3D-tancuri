#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texcoord;
out vec3 frag_position;


void main()
{
    // TODO(student): Send output to fragment shader

    frag_color = v_color;
	frag_normal = v_normal * (abs(tan(Time)));
    frag_texcoord = v_texcoord;
    frag_position = v_position;

    // TODO(student): Compute gl_Position

    gl_Position = Projection * View * Model * vec4(frag_position, 1.0);

}
