#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;


void main()
{

	mat4 translate;

	translate = mat4(1.0, 0.0, 0.0, 0.1, 
					  0.0, 1.0, 0.0, 0.4, 
					  0.0, 0.0, 1.0, -0.1,  
					  0.0, 0.0, 0.0, 1.0);

    gl_Position = translate * projection * model * vec4(position, 1.0f);
}