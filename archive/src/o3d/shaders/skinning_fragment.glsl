uniform sampler2D tex;
varying vec4 color;

void main()
{
	gl_FragColor = texture2D(tex,gl_TexCoord[0].st) * color;
}
