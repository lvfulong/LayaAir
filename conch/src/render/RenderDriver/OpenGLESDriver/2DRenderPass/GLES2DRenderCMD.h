#ifndef __GLES2DRenderCMD_H_
#define __GLES2DRenderCMD_H_
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderCMD.h>
namespace laya{
	class GLESRenderContext2D;
	class GLESRenderElement2D;
	class GLESInternalRT;


	class GLESSetRendertarget2DCMD :GLESRenderCMD {
	public:
		GLESSetRendertarget2DCMD();
		~GLESSetRendertarget2DCMD();
		void apply(void* context) override;
		void clearColorValue(const Color& color) { _clearColorValue = color; };
		void setRT(GLESInternalRT* rt) { _rt = rt; };
		void setClearColor(bool value) { _clearColor = value; };
		void setinvertY(bool value) { _invertY = value; };
	private:
		GLESInternalRT* _rt;
		Color _clearColorValue;
		bool _clearColor;
		bool _invertY;
	};

	class GLESDraw2DElementCMD :GLESRenderCMD {
	public:
		GLESDraw2DElementCMD();
		~GLESDraw2DElementCMD();
		void apply(void* context) override;
		void clearElement();
		void addOneElement(GLESRenderElement2D* element);
	private:
		std::vector<GLESRenderElement2D*> _elements;
	};

	class GLESBlit2DQuadCMD :GLESRenderCMD {
	public:
		GLESBlit2DQuadCMD();
		~GLESBlit2DQuadCMD();
		void apply(void* context) override;
		void setRenderElement(GLESRenderElement2D* element) { _element = element; };
		void setOffsetScale(const Vector4& v4) {_offsetScale = v4;};
		void setDest(GLESInternalRT* dest) { _dest = dest; };
		void setSource(GLESInternalTex* source);
	private:
		Vector4 _sourceTexelSize;
		Vector4 _offsetScale;
		GLESInternalRT* _dest = nullptr;
		GLESInternalTex* _source = nullptr;
		GLESRenderElement2D* _element = nullptr;
	};

}
#endif
