#include "RTSkinRenderNode.h"
#include "render/Utils3D.h"
#include "Bindings/LayaAir/3D/JSTransform.h"
#include <render/Property.h>

namespace laya
{
	void RTSkinRenderNode::computeSkinnedData(int loopCount)
	{
		//var bindPoses : Matrix4x4[] = this._cacheMesh._inverseBindPoses;
		//var pathMarks : skinnedMatrixCache[] = this._cacheMesh._skinnedMatrixCaches;
		for (int i = 0, n = subMeshCount; i < n; i++) {
			std::vector<std::pair<uint16_t*, int>>* subMeshBoneIndices = &boneIndicesList[i];
			std::vector < std::pair<float*, int>>* subData = &skinnedData[i];
			for (int j = 0, m  = subMeshBoneIndices->size(); j < m; j++) {
				_computeSubSkinnedData(subMeshBoneIndices->at(j).first, subMeshBoneIndices->at(j).second, subData->at(j).first,loopCount);
			}
		}
	}

	void RTSkinRenderNode::_computeSubSkinnedData(uint16_t* boneIndices, int boneIndicesLength, float* data, int loopCount) {
		for (int k  = 0, q = boneIndicesLength; k < q; k++) {
			int index = boneIndices[k];
			if (_skinnedDataLoopMarks[index] == loopCount) {
				SkinnedMatrixCache c  = _skinnedMatrixCache[index];
				float* preData = skinnedData[c.subMeshIndex].at(c.batchIndex).first;
				int srcIndex = c.batchBoneIndex * 16;
				int dstIndex = k * 16;
				for (int d = 0; d < 16; d++) {
					data[dstIndex + d] = preData[srcIndex + d];
					
				}
					
			}
			else {
				JSTransform* bone = bones[index];
				if (bone) {
					laya::Utils3D::_mulMatrixArray(bone->getWorldMatrix().elements, inverseBindPose[index].elements, 0, data, k * 16);
				}
					
				_skinnedDataLoopMarks[index] = loopCount;
			}
		}
	}

	void RTSkinRenderNode::_renderUpdate(GLESRenderContext3D* context) {
		_applyReflection();
		_applyLightProb();
		if (rootBoneTransform != nullptr) {
			worldParams.x = 1;
			shaderData->setMatrix4x4(Sprite3DProperty::WORLDMATRIX, Matrix4x4::DEFAULT);
		}
		else
		{
			shaderData->setMatrix4x4(Sprite3DProperty::WORLDMATRIX, ownerTransform->getWorldMatrix());
			worldParams.x = ownerTransform->getFrontFaceValue();
		}
		shaderData->setVector(Sprite3DProperty::WORLDINVERTFRONT, this->worldParams);
		/*this.shaderData.setMatrix4x4(Sprite3D.WORLDMATRIX, mat);
		this.shaderData.setVector(Sprite3D.WORLDINVERTFRONT, worldParams);*/
	}

	void RTSkinRenderNode::resizeSkinnedData(uint32_t dataArrayLength)
	{
		skinnedData.clear();
		skinnedData.resize(dataArrayLength);
	}

	void RTSkinRenderNode::setSkinnedDataByIndex(uint32_t index, JSValueAsParam pData)
	{
		std::vector<JsValue> vecDatas;
		__JsArray<JsValue>::FromJsArray(pData, vecDatas);
		std::vector<std::pair<float*, int>> skinData ;
		for (int i = 0, size = vecDatas.size(); i < size; i++)
		{
			char* pArrayBufferPtr = NULL;
			int nABLen = 0;
			bool bIsArrayBuffer = extractJSAB(vecDatas[i], pArrayBufferPtr, nABLen);
			float* data = static_cast<float*>(static_cast<void*> (pArrayBufferPtr));
			
			skinData.push_back(std::make_pair(data, nABLen / 4));
		}
		skinnedData[index] = skinData;
	}

	void RTSkinRenderNode::clearBoneTransform()
	{
		bones.clear();
	}

	void RTSkinRenderNode::addBoneTransform(JSTransform* value)
	{
		bones.push_back(value);
	}

	void RTSkinRenderNode::resizeCacheMeshInverseBindPoes(uint32_t value)
	{
		inverseBindPose.resize(value);
		_skinnedDataLoopMarks.resize(value);
	}

	void RTSkinRenderNode::setinverseBindPoseDataByIndex(uint32_t index, const Matrix4x4& value)
	{
		inverseBindPose[index] = value;
	}

	void RTSkinRenderNode::resizeMatrixCache(uint32_t value)
	{
		_skinnedMatrixCache.resize(value);
	}

	void RTSkinRenderNode::setMatrixCacheByIndex(uint32_t index, int batchBoneIndex, int batchIndex, int subMeshIndex)
	{
		SkinnedMatrixCache cacheData;
		cacheData.batchBoneIndex = batchBoneIndex;
		cacheData.batchIndex = batchIndex;
		cacheData.subMeshIndex = subMeshIndex;
		_skinnedMatrixCache[index] = cacheData;
	}

	void RTSkinRenderNode::resizeBoneIndicesList(uint32_t value)
	{
		boneIndicesList.resize(value);
	}

	void RTSkinRenderNode::setBoneIndicesList(uint32_t index, JSValueAsParam pData)
	{
		std::vector<JsValue> vecDatas;
		__JsArray<JsValue>::FromJsArray(pData, vecDatas);
		std::vector<std::pair<uint16_t*, int>> boneIndexData;
		for (int i = 0, size = vecDatas.size(); i < size; i++)
		{
			char* pArrayBufferPtr = NULL;
			int nABLen = 0;
			bool bIsArrayBuffer = extractJSAB(vecDatas[i], pArrayBufferPtr, nABLen);
			uint16_t* data = static_cast<uint16_t*>(static_cast<void*> (pArrayBufferPtr));

			boneIndexData.push_back(std::make_pair(data, nABLen / 2));
		}
		boneIndicesList[index] = boneIndexData;
	}






}// namespace laya