#ifndef __MeshVG_H__
#define __MeshVG_H__

#include <stdio.h>
#include "Matrix.h"
#include <map>
#include <vector>
#include <stack>
#include "Mesh2D.h"
#include <array>
#include <cstring>
namespace laya
{
	class MeshVG : public Mesh2D
	{	
	public:
		enum
		{
			const_stride = 12,
		};
		MeshVG(WebGLEngine* engine);
		~MeshVG();
		static MeshVG* getAMesh(WebGLEngine* engine, bool mainctx);
        static void onDestroy();
		void recycle();
		void addVertAndIBToMesh(const std::vector<float>& points, uint32_t rgba, const std::vector<uint16_t>& ib);
		void addVertAndIBToMesh(const std::vector<std::array<float, 2> >& points, uint32_t rgba, const std::vector<uint16_t>& ib);
	public:
		static std::stack<MeshVG*> _POOL;
	};
}
#endif //__MeshVG_H__
