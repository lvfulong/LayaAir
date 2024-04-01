#include "GLESInstanceRenderBatch.h"
#include "GLESInstanceRenderElement3D.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTReflectionProb.h"
#include "render/3D/RenderObjs/RuntimeOBJ/RenderModuleData/RTVolumetricGI.h"
#include "render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
namespace laya
{
	GLESInstanceRenderBatch::GLESInstanceRenderBatch() :_recoverList(JCSingletonList<GLESInstanceRenderElement3D*>(false))
	{
		
	}
	GLESInstanceRenderBatch::~GLESInstanceRenderBatch()
	{
	}
	BatchMark* GLESInstanceRenderBatch::getBathMark(GLESRenderElement3D* element)
	{
		RTBaseRenderNode* renderNode = element->owner;
		GLESRenderGeometryElement* geometry = element->geometry;

		bool invertFrontFace = (element->transform!=nullptr) ? element->transform->_isFrontFaceInvert() : false;
		uint32_t invertFrontFaceFlag = invertFrontFace ? 1 : 0;
		uint32_t receiveShadowFlag = renderNode->receiveShadow ? 1 : 0;
		uint32_t geometryFlag = geometry->_id;
		uint32_t materialFlag = element->materialId;

		uint32_t renderId = (materialFlag << 17) + (geometryFlag << 2) + (invertFrontFaceFlag << 1) + (receiveShadowFlag);
		
		uint32_t reflectFlag = (renderNode->probeReflection!=nullptr ? renderNode->probeReflection->_id : -1) + 1;
		uint32_t lightmapFlag = renderNode->lightmapIndex + 1;
		uint32_t lightProbeFlag = (renderNode->volumetricGI!=nullptr ? renderNode->volumetricGI->_id : -1) + 1;
		uint32_t giId = (reflectFlag << 10) + (lightmapFlag << 20) + lightProbeFlag;
		
		//let data = this._batchQpaqueMarks[renderId] || (this._batchQpaqueMarks[renderId] = {});
		if (_batchQpaqueMarks.find(renderId) == _batchQpaqueMarks.end()) {
			std::pair<uint32_t, std::unordered_map<uint32_t, BatchMark*>*> part(renderId, new std::unordered_map<uint32_t, BatchMark*>());
			_batchQpaqueMarks.insert(part);
			
		}
		std::unordered_map<uint32_t, BatchMark*>* map = _batchQpaqueMarks.at(renderId);
		if (map->find(giId) == map->end()) {
			
			std::pair<uint32_t, BatchMark*> gipart(giId, new BatchMark());
			map->insert(gipart);
		}
		return map->at(giId);
	}

	void GLESInstanceRenderBatch::batch(JCSingletonList<GLESRenderElement3D*> &elements)
	{
		if (!LayaGL::m_pWebglEngine->getCapable(RenderCapable::DrawElement_Instance)) {
			return;
		}
		uint32_t elementCount = elements.getLength();
		std::vector<GLESRenderElement3D*>& elementArray = elements.m_vElements;
		elements.setLength(0);
		_updateCountMark++;
		for (int i = 0; i < elementCount; i++) {
			GLESRenderElement3D* element = elementArray[i];
			if (element->canDynamicBatch) {
				BatchMark* instanceMark = getBathMark(element);
				if (_updateCountMark == instanceMark->updateMark) {
					int instanceIndex = instanceMark->indexInList;
					if (instanceMark->batched) {
						GLESInstanceRenderElement3D* originElement = reinterpret_cast<GLESInstanceRenderElement3D*>( elementArray[instanceIndex]);
						// 达到 最大 instance 数量 放弃合并 // todo
						if (originElement->_instanceElementList.size() == GLESInstanceRenderElement3D::maxInstanceCount) {
							instanceMark->indexInList = elements.getLength();
							instanceMark->batched = false;
							elements.add(element);
						}
						else {
							// 加入合并队列
							originElement->_instanceElementList.push_back(element);
						}
					}
					else {
						GLESRenderElement3D* originElement = elementArray[instanceIndex];
						// 替换 renderElement
						GLESInstanceRenderElement3D* instanceRenderElement = GLESInstanceRenderElement3D::create();
						_recoverList.add(instanceRenderElement);
						reinterpret_cast<GLESRenderElement3D*>( instanceRenderElement)->subshader = element->subshader;
						reinterpret_cast<GLESRenderElement3D*>(instanceRenderElement)->materialShaderData = element->materialShaderData;
						reinterpret_cast<GLESRenderElement3D*>(instanceRenderElement)->materialRenderQueue = element->materialRenderQueue;
						reinterpret_cast<GLESRenderElement3D*>(instanceRenderElement)->renderShaderData = element->renderShaderData;
						reinterpret_cast<GLESRenderElement3D*>(instanceRenderElement)->owner = element->owner;
						instanceRenderElement->setGeometry(element->geometry);

						std::vector<GLESRenderElement3D*> *list = &instanceRenderElement->_instanceElementList;
						list->resize(0);
						list->push_back(originElement);
						list->push_back(element);
						elementArray[instanceIndex] =  instanceRenderElement;
						instanceMark->batched = true;
						instanceRenderElement->_invertFrontFace = element->transform!=nullptr ? element->transform->_isFrontFaceInvert() : false;
					}
				}
				else
				{
					instanceMark->updateMark = _updateCountMark;
					instanceMark->indexInList = elements.getLength();
					instanceMark->batched = false;
					elements.add(element);
				}
			}
			else {
				elements.add(element);
			}
		}

	}


	void GLESInstanceRenderBatch::clearRenderData()
	{
		for (int i = 0, n = _recoverList.getLength(); i < n; i++) {
			GLESInstanceRenderElement3D* element = _recoverList.m_vElements[i];
			element->clearRenderData();
		}
	}
	void GLESInstanceRenderBatch::recoverData()
	{
		for (int i = 0, n = _recoverList.getLength(); i < n; i++) {
			GLESInstanceRenderElement3D* element = _recoverList.m_vElements[i];
			element->recover();
		}
		_recoverList.setLength(0);
	}
}
