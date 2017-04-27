#version 330 core
out vec4 color;
in vec2 tc;
in vec3 pos;
uniform sampler2D Texture0;
void main()
{
	//color=vec4(gray,gray,gray,1.0f);
	color=texture(Texture0,tc);
}