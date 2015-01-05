#ifndef _CE_BASE_RENDER_CONTEXT_H_
#define _CE_BASE_RENDER_CONTEXT_H_

//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Matrix4x4.h>

namespace ce
{
	typedef struct RenderContext
	{
		int_canvas width, height;
		bool isCanvas, useShaders;
		Matrix4x4<float> projectionMatrix, modelViewMatrix, mvpMatrix;
	} RenderContext;
}

#endif
