varying vec4 color;

void main()
{
	color = vec4(gl_Vertex[0], gl_Vertex[1], gl_Vertex[2], 1.0);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
