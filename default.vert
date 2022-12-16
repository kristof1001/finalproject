#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
// Outputs the color for the Fragment Shader
out vec3 color;
out vec2 texCoord;
// Controls the scale of the vertices
uniform float scale;
// 3D
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main()
{
	// Outputs the positions/coordinates of all vertices
	// gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	texCoord = aTex;
}