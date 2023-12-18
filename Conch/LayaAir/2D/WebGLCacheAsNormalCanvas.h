#ifndef __WebGLCacheAsNormalCanvas_H__
#define __WebGLCacheAsNormalCanvas_H__

#include <stdio.h>
#include <stack>
#include "Matrix.h"
#include <vector>
#include "MeshTexture.h"
#include "BlendMode.h"
#include "Path.h"
#include "Rectangle.h"
#include "CharRenderInfo.h"

namespace laya
{
	class ISubmit;
	class Context2D;
	class SubmitBase;
	class MeshQuadTexture;
	class MeshVG;
	class MeshTexture;
	class WebGLCacheAsNormalCanvas
	{
	public:
		WebGLCacheAsNormalCanvas(Context2D* ctx);
		~WebGLCacheAsNormalCanvas();
		void startRec();
		void endRec();
		bool isCacheValid();
		bool isTextNeedRestore();
		void flushsubmit();
		Context2D* m_context;
		int m_submitStartPos = 0;
		std::vector<ISubmit*>* m_submits = nullptr;
		std::vector<Mesh2D*> m_meshlist;
		MeshQuadTexture* m_mesh;
		MeshVG* m_pathMesh;	
		MeshTexture* m_triangleMesh;

		MeshQuadTexture* m_oldMesh;
		MeshVG* m_oldPathMesh;
		MeshTexture* m_oldTriMesh;
		std::vector<Mesh2D*> m_oldMeshList;
		float m_oldTx = 0;
		float m_oldTy = 0;
		Matrix m_cachedClipInfo;
		Matrix m_invMat;
		std::vector<std::shared_ptr<CharRenderInfo>> m_touches;
	};
}
#endif //__WebGLCacheAsNormalCanvas_H__