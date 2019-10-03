#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normalIn;
layout (location = 3) in vec4 ColorIn;

//layout (location = 0) out vec3 pos;
layout (location = 3) out vec4 MyColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rotate;

out vec3 normal;

void main()
{
     gl_Position = projection * view * model * vec4(position, 1.0f);  
	 vec4 normal4 = vec4(normalIn, 1.0f);
	 //vec4 normal4 = vec4(0.0f,1.0f,0.0f, 1.0f);
	 vec4 wNormal = rotate * normal4;
     normal = vec3(wNormal);
	 MyColor = ColorIn;
	 
	 //pos = vec3(model * vec4(position, 1.0f));
}