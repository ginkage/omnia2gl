#ifdef GL_ES
precision highp float;
#endif

attribute vec4 Vertex;
attribute vec4 Color;
attribute vec4 MultiTexCoord0;
attribute vec3 PaletteIndex;
attribute vec3 PaletteWeight;

uniform mat4 MatrixPalette[9];
uniform mat4 ProjectionMatrix;
uniform int PaletteSize;

varying vec4 FrontColor;
varying vec4 TexCoord0;

void main()
{
//	int idx;
//	idx = int(PaletteIndex.x);
	vec4 vPos = (MatrixPalette[0] * Vertex) * PaletteWeight.x;
	if (PaletteSize > 1)
	{
//		idx = int(PaletteIndex.y);
		vPos += (MatrixPalette[1] * Vertex) * PaletteWeight.y;
		if (PaletteSize > 2)
		{
//			idx = int(PaletteIndex.z);
			vPos += (MatrixPalette[2] * Vertex) * PaletteWeight.z;
		}
	}
	gl_Position = ProjectionMatrix * vPos;
	FrontColor = Color;
	TexCoord0 = MultiTexCoord0;
}

