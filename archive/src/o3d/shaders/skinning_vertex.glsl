uniform mat4 boneMatrices[128];
attribute vec4 weight;
attribute vec4 index;
attribute vec4 numBones;
varying vec4 color;

void main()
{
	color = gl_Color;
	int count = int(numBones.x);
	vec4 curIndex = index;
	vec4 curWeight = weight;
	vec4 pos = vec4(0.0,0.0,0.0,0.0);
	for(int a = 0; a < count; a++)
	{
		mat4 m44 = boneMatrices[int(curIndex.x)];
		pos = pos + curWeight.x * (m44 * gl_Vertex);
		curIndex = curIndex.yzwx;
		curWeight = curWeight.yzwx;
	}
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}
