#ifndef __GLESInstanceRenderElement3D_H__
#define __GLESInstanceRenderElement3D_H__

#include <utils/JCSingletonList.h>
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h"
namespace laya
{

	class GLESRenderElement3D;
	class GLESVertexBuffer;
	
	struct GLESInstanceStateInfo
	{
	public:
		GLESInstanceStateInfo() {};
		~GLESInstanceStateInfo(){};
		GLESBufferState* state = nullptr;
		GLESVertexBuffer* worldInstanceVB = nullptr;
		GLESVertexBuffer* lightmapScaleOffsetVB = nullptr;
		GLESVertexBuffer* simpleAnimatorVB = nullptr;
	};

	class GLESInstanceRenderElement3D : public GLESRenderElement3D
	{
	public:
		static uint32_t maxInstanceCount;
		static GLESInstanceStateInfo* getInstanceBufferState(GLESRenderGeometryElement* geometry, BaseRenderType renderType, RTDefineDatas* spriteDefine);
		static GLESInstanceRenderElement3D* create();

	private:
		static std::vector<GLESInstanceRenderElement3D*> _pool;
		static std::unordered_map<uint32_t, GLESInstanceStateInfo*> _instanceBufferStateMap;
		static std::unordered_map<uint32_t, std::vector<std::vector<float>*>*> _bufferPool;
		static std::vector<float>* _instanceBufferCreate(uint32_t length);

	public:
		GLESInstanceRenderElement3D();
		~GLESInstanceRenderElement3D();
		void addUpdateBuffer(GLESVertexBuffer* vb, uint32_t length);
		std::vector<float>* getUpdateData(uint32_t index, uint32_t length);
		void setGeometry(GLESRenderGeometryElement* geometry);
		void clearRenderData();
		void recover();
	protected:
		bool _getInvertFront() override;
		void _compileShader(GLESRenderContext3D* context) override;
		void drawGeometry(GLESShaderInstance* shaderIns) override;
	private:
		void _updateInstanceData();

	public:
		std::vector<GLESRenderElement3D*> _instanceElementList{};
		uint32_t drawCount;
		bool _invertFrontFace = true;
		uint32_t updateNums;
	private:
		std::vector<GLESVertexBuffer*> _vertexBuffers{};
		std::unordered_map<uint32_t,std::vector<float>*> _updateData{};
		std::vector<uint32_t> _updateDataNum;
		GLESInstanceStateInfo* _instanceStateInfo;
	};
} // namespace laya
#endif