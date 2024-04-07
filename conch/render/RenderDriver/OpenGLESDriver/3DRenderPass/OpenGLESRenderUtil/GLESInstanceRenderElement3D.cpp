#include "GLESInstanceRenderElement3D.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESVertexBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h"
#include <render/Property.h>
#include <render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESRenderGeometryElement.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
namespace laya
{
	uint32_t GLESInstanceRenderElement3D::maxInstanceCount = 1024;
	std::vector<GLESInstanceRenderElement3D*> GLESInstanceRenderElement3D::_pool = {};
	std::unordered_map<uint32_t, GLESInstanceStateInfo*> GLESInstanceRenderElement3D::_instanceBufferStateMap = {};
	std::unordered_map<uint32_t, std::vector<std::vector<float>*>*> GLESInstanceRenderElement3D::_bufferPool = {};

	std::vector<float>* GLESInstanceRenderElement3D::_instanceBufferCreate(uint32_t length) {
		if (GLESInstanceRenderElement3D::_bufferPool.find(length) == GLESInstanceRenderElement3D::_bufferPool.end())
			GLESInstanceRenderElement3D::_bufferPool[length] = new std::vector<std::vector<float>*>();
		std::vector<std::vector<float>*>* lengthMap = GLESInstanceRenderElement3D::_bufferPool.at(length);
		if (lengthMap->size() > 0) {
			std::vector<float>* dataArray = lengthMap->back();
			lengthMap->pop_back();
			return dataArray;
		}
		else
		{
			std::vector<float>* dataArray = new std::vector<float>();
			dataArray->resize(length);
			return dataArray;
		}
	}

	GLESInstanceStateInfo* GLESInstanceRenderElement3D::getInstanceBufferState(GLESRenderGeometryElement* geometry, BaseRenderType renderType, RTDefineDatas* spriteDefine) {
		if (GLESInstanceRenderElement3D::_instanceBufferStateMap.find(geometry->_id) == GLESInstanceRenderElement3D::_instanceBufferStateMap.end()) {
			GLESInstanceStateInfo* stateinfo = new GLESInstanceStateInfo();
			stateinfo->state = new GLESBufferState();
			std::vector<GLESVertexBuffer*> vertexArray = geometry->_bufferState->_vertexBuffers;
			GLESVertexBuffer* worldMatVertex = new GLESVertexBuffer(BufferTargetType::ARRAY_BUFFER, BufferUsage::Dynamic);
			worldMatVertex->setDataLength(GLESInstanceRenderElement3D::maxInstanceCount * 16 * 4);
			worldMatVertex->_shaderValues = *LayaGL::m_pWebglEngine->getGlobalVertexDeclaration("instanceWorldMatrixDeclaration");
			worldMatVertex->_instanceBuffer = true;
			vertexArray.push_back(worldMatVertex);
			stateinfo->worldInstanceVB = worldMatVertex;
			switch (renderType) {
			case BaseRenderType::MeshRender:
				if (spriteDefine->has(MeshSprite3DShaderDeclaration::SHADERDEFINE_UV1)) {
					GLESVertexBuffer* instanceLightMapVertexBuffer = new GLESVertexBuffer(BufferTargetType::ARRAY_BUFFER, BufferUsage::Dynamic);
					instanceLightMapVertexBuffer->setDataLength(GLESInstanceRenderElement3D::maxInstanceCount * 4 * 4);
					instanceLightMapVertexBuffer->_shaderValues = *LayaGL::m_pWebglEngine->getGlobalVertexDeclaration("instanceLightMapScaleOffsetDeclaration");
					instanceLightMapVertexBuffer->_instanceBuffer = true;
					vertexArray.push_back(instanceLightMapVertexBuffer);
					stateinfo->lightmapScaleOffsetVB = instanceLightMapVertexBuffer;
				}
				break;
			case BaseRenderType::SimpleSkinRender: 
				{
				GLESVertexBuffer* instanceSimpleAnimatorBuffer = new GLESVertexBuffer(BufferTargetType::ARRAY_BUFFER, BufferUsage::Dynamic);
				instanceSimpleAnimatorBuffer->setDataLength(GLESInstanceRenderElement3D::maxInstanceCount * 4 * 4);
				instanceSimpleAnimatorBuffer->_shaderValues = *LayaGL::m_pWebglEngine->getGlobalVertexDeclaration("instanceSimpleAnimatorDeclaration");
				instanceSimpleAnimatorBuffer->_instanceBuffer = true;
				vertexArray.push_back(instanceSimpleAnimatorBuffer);
				stateinfo->simpleAnimatorVB = instanceSimpleAnimatorBuffer;
				}
				break;
			default:

				break;
			}
			stateinfo->state->applyState(vertexArray, geometry->_bufferState->_bindedIndexBuffer);
			GLESInstanceRenderElement3D::_instanceBufferStateMap[geometry->_id] = stateinfo;
			return stateinfo;
		}
		else
			return GLESInstanceRenderElement3D::_instanceBufferStateMap.at(geometry->_id);
	}

	GLESInstanceRenderElement3D* GLESInstanceRenderElement3D::create() {
		if (GLESInstanceRenderElement3D::_pool.size() > 0) {
			GLESInstanceRenderElement3D* element = GLESInstanceRenderElement3D::_pool.back();
			GLESInstanceRenderElement3D::_pool.pop_back();
			return element;
		}
		else
		{
			return new GLESInstanceRenderElement3D();
		}

	}

	GLESInstanceRenderElement3D::GLESInstanceRenderElement3D()
	{
		drawCount = 0;
		updateNums = 0;
		isRender = true;
	}

	GLESInstanceRenderElement3D::~GLESInstanceRenderElement3D()
	{

	}

	void GLESInstanceRenderElement3D::addUpdateBuffer(GLESVertexBuffer* vb, uint32_t length)
	{
		if (_vertexBuffers.size() < updateNums + 1) {
			_vertexBuffers.resize(updateNums + 1);
			_updateDataNum.resize(updateNums + 1);
		}
		_vertexBuffers[updateNums] = vb;
		_updateDataNum[updateNums] = length;
		updateNums++;
	}

	std::vector<float>* GLESInstanceRenderElement3D::getUpdateData(uint32_t index, uint32_t length)
	{
		_updateData[index] = GLESInstanceRenderElement3D::_instanceBufferCreate(length);
		return _updateData.at(index);
	}

	void GLESInstanceRenderElement3D::setGeometry(GLESRenderGeometryElement* value)
	{
		if (geometry == nullptr) {
			geometry = new GLESRenderGeometryElement();
			geometry->setMeshTopology(value->_mode);
			geometry->setDrawType(value->m_nDrawType);
		}
		value->cloneTo(geometry);
		geometry->setDrawType(DrawType::DrawElementInstance);
		_instanceStateInfo = GLESInstanceRenderElement3D::getInstanceBufferState(value, (BaseRenderType)owner->renderNodeType, renderShaderData->_defineDatas);
		geometry->_bufferState = _instanceStateInfo->state;
	}

	void GLESInstanceRenderElement3D::clearRenderData()
	{
		drawCount = 0;
		updateNums = 0;
		_vertexBuffers.resize(0);
		for (std::unordered_map<uint32_t, std::vector<float>*>::iterator it = _updateData.begin(); it != _updateData.end(); ++it) {
			GLESInstanceRenderElement3D::_bufferPool.at(it->second->size())->push_back(it->second);
		}
		_updateData.clear();
		_updateDataNum.resize(0);
	}

	void GLESInstanceRenderElement3D::recover()
	{
		GLESInstanceRenderElement3D::_pool.push_back(this);
		_instanceElementList.resize(0);
	}

	bool GLESInstanceRenderElement3D::_getInvertFront()
	{
		return _invertFrontFace;
	}

	void GLESInstanceRenderElement3D::_compileShader(GLESRenderContext3D* context)
	{
		std::vector<RTShaderPass*> passes = subshader->shaderpasses;
		_clearShaderInstance();
		for (uint32_t j = 0, m = passes.size(); j < m; j++)
		{
			RTShaderPass* pass = passes[j];
			// NOTE:this will cause maybe a shader not render but do prepare before£¬but the developer can avoide this
			// manual,for example shaderCaster=false.
			if (pass->pipelineMode != context->pipelineMode)
				continue;

			RTDefineDatas* comDef = GLESRenderElement3D::_compileDefines;
			if (context->sceneData)
			{
				context->sceneData->_defineDatas->cloneTo(comDef);
			}
			else
			{
				context->globalConfigShaderData->cloneTo(comDef);
			}

			if (context->cameraData != nullptr)
				comDef->addDefineDatas(context->cameraData->_defineDatas);
			if (renderShaderData != nullptr)
			{
				comDef->addDefineDatas(renderShaderData->_defineDatas);
				pass->nodeCommonMap = owner->commonUniformMap;
			}
			else
			{
				pass->nodeCommonMap.clear();
			}
			comDef->addDefineDatas(materialShaderData->_defineDatas);
			comDef->add(MeshSprite3DShaderDeclaration::SHADERDEFINE_GPU_INSTANCE);
			_updateInstanceData();
			RTShaderPass::CacheShaderItem* item = pass->getCacheShader(comDef);
			GLESShaderInstance* shader;
			if (item)
				shader = item->_glesShaderInstance;
			else
				shader = pass->callCreateShaderInstanceFunction();
			assert(shader != nullptr);
			_addShaderInstance(shader);
		}
	}

	void GLESInstanceRenderElement3D::drawGeometry(GLESShaderInstance* shaderIns)
	{
		for (int i = 0; i < updateNums; i++) {
			GLESVertexBuffer* buffer = _vertexBuffers[i];
			if (buffer == nullptr)
				break;
			std::vector<float>* data = _updateData[i];

			buffer->orphanStorage();
			buffer->setData(reinterpret_cast<char*>(data->data()), data->size() * 4, 0, 0, drawCount * _updateDataNum[i] * 4);
		}
		LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(geometry);
	}

	void GLESInstanceRenderElement3D::_updateInstanceData()
	{
		float* worldMatrixData = nullptr;
		switch (static_cast<BaseRenderType>(owner->renderNodeType)) {

		case BaseRenderType::MeshRender: 
		{
			worldMatrixData = getUpdateData(0, GLESInstanceRenderElement3D::maxInstanceCount * 16)->data();

			addUpdateBuffer(_instanceStateInfo->worldInstanceVB, 16);
			drawCount = _instanceElementList.size();
			geometry->setInstanceCount(drawCount);
			for (uint32_t i = 0; i < drawCount; i++) {
				memcpy(worldMatrixData + i * 16, _instanceElementList[i]->transform->getWorldMatrix().elements, 16 * sizeof(float));
			}
			bool haveLightMap = renderShaderData->hasDefine(RenderableSprite3D::SAHDERDEFINE_LIGHTMAP) && renderShaderData->hasDefine(MeshSprite3DShaderDeclaration::SHADERDEFINE_UV1);
			if (haveLightMap) {
				float* lightMapData = getUpdateData(1, 4 * GLESInstanceRenderElement3D::maxInstanceCount)->data();
				for (uint32_t i = 0; i < drawCount; i++) {
					Vector4* v4 = &(_instanceElementList[i]->owner->lightmapScaleOffset);
					uint32_t offset = i * 4;
					lightMapData[offset] = v4->x;
					lightMapData[offset + 1] = v4->y;
					lightMapData[offset + 2] = v4->z;
					lightMapData[offset + 3] = v4->w;
				}
				addUpdateBuffer(_instanceStateInfo->lightmapScaleOffsetVB, 4);
			}
		}
		break;
		case BaseRenderType::SimpleSkinRender: 
		{
			worldMatrixData = getUpdateData(0, GLESInstanceRenderElement3D::maxInstanceCount * 16)->data();
			addUpdateBuffer(_instanceStateInfo->worldInstanceVB, 16);
			drawCount = _instanceElementList.size();
			geometry->setInstanceCount(drawCount);
			for (uint32_t i = 0; i < drawCount; i++) {
				memcpy(worldMatrixData += i * 16, _instanceElementList[i]->transform->m_worldMatrix.elements, 16 * sizeof(float));
			}
			//simpleAnimationData
			float* simpleAnimatorData = getUpdateData(1, 4 * GLESInstanceRenderElement3D::maxInstanceCount)->data();
			for (uint32_t i = 0; i < drawCount; i++) {
				Vector4* v4 = _instanceElementList[i]->renderShaderData->getVector(RenderableSprite3D::SIMPLESKINNEDMESHRENDERER_SIMPLE_SIMPLEANIMATORPARAMS);
				uint32_t offset = i * 4;
				simpleAnimatorData[offset] = v4->x;
				simpleAnimatorData[offset + 1] = v4->y;
				simpleAnimatorData[offset + 2] = v4->z;
				simpleAnimatorData[offset + 3] = v4->w;
			}
			addUpdateBuffer(_instanceStateInfo->simpleAnimatorVB, 4);
		}
			break;
		default:
			break;
		}
	}
}

	
	
