#version 330 core
out vec4 color;
in vec2 tc;
uniform sampler2D Texture0;
void main()
{
	color=texture(Texture0,tc);
}